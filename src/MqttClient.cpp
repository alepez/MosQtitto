#include "MqttClient.hpp"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <mosquitto.h>
#include <unistd.h>

namespace {

void on_connect_wrapper(struct mosquitto*, void* that, int rc) {
  MqttClient* m = reinterpret_cast<MqttClient*>(that);
  m->_onConnect(rc);
}

void on_disconnect_wrapper(struct mosquitto*, void* that, int rc) {
  MqttClient* m = reinterpret_cast<MqttClient*>(that);
  m->_onDisconnect(rc);
}

void on_message_wrapper(struct mosquitto*, void* that, const struct mosquitto_message* message) {
  MqttClient* m = reinterpret_cast<MqttClient*>(that);
  m->_onMessage(message);
}

std::string generateRandomClientId() {
  std::string id;
  id.resize(32);
  for (auto& c : id) {
    c = (::rand() % 25) + 'a';
  }
  return id;
}

} /* namespace _ */

MqttClient::MqttClient(QObject* parent)
    : QThread{parent}
    , isConnected_{false} {

  // qDebug() << "MqttClient ctor";

  /* See main.cpp */
  // mosquitto_lib_init();

  /* We need these functions to forward signal with using queued connection */

  auto onConnected = [this]() {
    isConnected_ = true;
    emit connected();
    emit isConnectedChanged();
  };

  auto onDisconnected = [this]() {
    isConnected_ = false;
    emit disconnected();
    emit isConnectedChanged();
  };

  auto onMessage = [this](QString topic, QByteArray payload) {
    /* emit with queued connection */
    emit message(topic, payload);
  };

  /* QueuedConnection is needed, so callbacks are handled outside of mosquitto thread */
  QObject::connect(this, &MqttClient::_connected, this, onConnected, Qt::QueuedConnection);
  QObject::connect(this, &MqttClient::_disconnected, this, onDisconnected, Qt::QueuedConnection);
  QObject::connect(this, &MqttClient::_message, this, onMessage, Qt::QueuedConnection);
}

MqttClient::~MqttClient() {
  /* Force disconnection and stop the thread */
  this->disconnect();

  /* See main.cpp */
  // mosquitto_lib_cleanup();
}

void MqttClient::connect() {
  this->QThread::start();
}

void MqttClient::disconnect() {
  continueRunning_ = false;
  if (!this->wait(2000)) {
    qDebug() << "Timeout while waiting for disconnection.";
  }
}

void MqttClient::run() {
  // qDebug() << "RUN";

  /* Mandatory options: */
  std::string hostname = options_["hostname"].toString().toStdString();
  int port = options_["port"].toInt();
  int keepalive = options_["keepalive"].toInt();

  /* Optional options */
  std::string username = options_.contains("username") ? options_["username"].toString().toStdString() : "";
  std::string password = options_.contains("password") ? options_["password"].toString().toStdString() : "";
  std::string clientId = options_.contains("clientId") ? options_["clientId"].toString().toStdString() : "";

  if (clientId.empty()) {
    // qDebug() << "Generating a random client id...";
    clientId = generateRandomClientId();
  }

  auto mosq = mosquitto_new(clientId.c_str(), true, this);

  if (!mosq) {
    throw std::runtime_error{"MqttClient: cannot instantiate mosquitto client"};
  }

  mosquitto_connect_callback_set(mosq, on_connect_wrapper);
  mosquitto_disconnect_callback_set(mosq, on_disconnect_wrapper);
  mosquitto_message_callback_set(mosq, on_message_wrapper);

  continueRunning_ = true;

  int rc = 0;

  if (!username.empty()) {
    rc = mosquitto_username_pw_set(mosq, username.c_str(), password.c_str());

    if (rc != MOSQ_ERR_SUCCESS) {
      qDebug() << "MqttClient: cannot set mosquitto username/password";
      mosquitto_destroy(mosq);
      return;
    }
  }

  // qDebug() << "MqttClient connecting...";
  rc = mosquitto_connect(mosq, hostname.c_str(), port, keepalive);

  if (rc != MOSQ_ERR_SUCCESS) {
    qDebug() << "MqttClient: cannot connect";
    mosquitto_destroy(mosq);
    return;
  }

  // qDebug() << "MqttClient connected, starting loop...";

  while (continueRunning_) {
    rc = mosquitto_loop(mosq, 1000, 1);

    if (continueRunning_ && rc) {
      qWarning() << "Connection error";
      QThread::sleep(1);
      mosquitto_reconnect(mosq);
    }

    mutex_.lock();

    while (!subscribeQueue_.isEmpty()) {
      const auto& subscription = subscribeQueue_.front();
      auto topic = std::get<0>(subscription);
      auto qos = std::get<1>(subscription);
      mosquitto_subscribe(mosq, nullptr, topic.toStdString().c_str(), qos);
      subscribeQueue_.pop_front();
    }

    while (!publishQueue_.isEmpty()) {
      const auto& subscription = publishQueue_.front();
      auto topic = std::get<0>(subscription);
      auto payload = std::get<1>(subscription);
      auto qos = std::get<2>(subscription);
      mosquitto_publish(mosq, nullptr, topic.toStdString().c_str(), payload.size(), payload.constData(), qos, false);
      publishQueue_.pop_front();
    }

    mutex_.unlock();
  }

  mosquitto_disconnect(mosq);

  mosquitto_destroy(mosq);

  // qDebug() << "RUN END";
}

void MqttClient::publish(QString topic, QByteArray payload, int qos) {
  mutex_.lock();
  publishQueue_.push_back(std::make_tuple(topic, payload, qos));
  mutex_.unlock();
}

void MqttClient::jsonPublish(QString topic, QJsonObject jpayload, int qos) {
  mutex_.lock();
  auto payload = QJsonDocument(jpayload).toJson(QJsonDocument::Compact);
  publishQueue_.push_back(std::make_tuple(topic, payload, qos));
  mutex_.unlock();
}

void MqttClient::subscribe(QString topic, int qos) {
  // qDebug() << "MqttClient subscribe to" << topic;
  mutex_.lock();
  subscribeQueue_.push_back(std::make_tuple(topic, qos));
  mutex_.unlock();
}

void MqttClient::_onConnect(int rc) {
  if (rc != MOSQ_ERR_SUCCESS) {
    qDebug() << "MqttClient Connection error";
    return;
  }

  // qDebug() << "MqttClient connected";
  emit _connected();
}

void MqttClient::_onDisconnect(int rc) {
  bool expected = rc == MOSQ_ERR_SUCCESS;

  if (expected) {
    // qDebug() << "Successfully disconnected";
  } else {
    qWarning() << "Unexpected disconnection";
  }

  emit _disconnected();
}

void MqttClient::_onMessage(const mosquitto_message* message) {
  // qDebug() << "MqttClient incoming message -> topic:" << message->topic << ", size:" << message->payloadlen;
  QString topic{message->topic};
  QByteArray payload{reinterpret_cast<const char*>(message->payload), message->payloadlen};

  emit _message(topic, payload);
  // qDebug() << "_message emitted";

  try {
    auto jdoc = QJsonDocument::fromJson(payload);
    auto jpayload = jdoc.object();

    emit _jsonMessage(topic, jpayload);
    // qDebug() << "_jsonMessage emitted";
  } catch (...) {
    /* Ignore parsing errors */
  }
}
