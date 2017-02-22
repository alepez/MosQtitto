#include "MqttClient.hpp"
#include <QDebug>
#include <QThread>
#include <mosquitto.h>
#include <thread>

namespace {

void on_connect_wrapper(struct mosquitto*, void* userdata, int rc) {
  MqttClient* m = reinterpret_cast<MqttClient*>(userdata);
  m->_on_connect(rc);
}

void on_disconnect_wrapper(struct mosquitto*, void* userdata, int rc) {
  MqttClient* m = reinterpret_cast<MqttClient*>(userdata);
  m->_on_disconnect(rc);
}

void on_publish_wrapper(struct mosquitto*, void* userdata, int mid) {
  MqttClient* m = reinterpret_cast<MqttClient*>(userdata);
  m->_on_publish(mid);
}

void on_message_wrapper(struct mosquitto*, void* userdata, const struct mosquitto_message* message) {
  MqttClient* m = reinterpret_cast<MqttClient*>(userdata);
  m->_on_message(message);
}

void on_subscribe_wrapper(struct mosquitto*, void* userdata, int mid, int qos_count, const int* granted_qos) {
  MqttClient* m = reinterpret_cast<MqttClient*>(userdata);
  m->_on_subscribe(mid, qos_count, granted_qos);
}

void on_unsubscribe_wrapper(struct mosquitto*, void* userdata, int mid) {
  MqttClient* m = reinterpret_cast<MqttClient*>(userdata);
  m->_on_unsubscribe(mid);
}

std::string generateRandomClientId() {
  std::string id;
  id.resize(16);
  for (auto& c : id) {
    c = (::rand() % 25) + 'a';
  }
  return id;
}

} /* namespace _ */

MqttClient::MqttClient(QObject* parent)
    : QObject{parent} {

  qDebug() << "MqttClient ctor";

  impl_ = std::shared_ptr<mosquitto>(mosquitto_new(generateRandomClientId().c_str(), true, this), mosquitto_destroy);

  if (!impl_) {
    QString message{"MqttClient: cannot instantiate mosquitto client"};
    throw std::runtime_error{message.toStdString().c_str()};
  }

  auto impl = impl_.get();

  mosquitto_connect_callback_set(impl, on_connect_wrapper);
  mosquitto_disconnect_callback_set(impl, on_disconnect_wrapper);
  mosquitto_publish_callback_set(impl, on_publish_wrapper);
  mosquitto_message_callback_set(impl, on_message_wrapper);
  mosquitto_subscribe_callback_set(impl, on_subscribe_wrapper);
  mosquitto_unsubscribe_callback_set(impl, on_unsubscribe_wrapper);

  /* Use std::thread instead of mosquitto thread (pthread) for portability */
  thread_ = std::make_shared<std::thread>([this]() { mosquitto_loop_forever(impl_.get(), -1, 1); });
}

MqttClient::~MqttClient() {
  /* Force disconnection and stop the thread */
  qDebug() << "MqttClient dtor";

  mosquitto_disconnect(impl_.get());
  thread_->join();
}

void MqttClient::connect(QString hostname, int port, int keepalive) {
  qDebug() << "MqttClient Connecting...";
  mosquitto_connect_async(impl_.get(), hostname.toStdString().c_str(), port, keepalive);
  // FIXME Handle error (rc)
}

void MqttClient::_on_connect(int rc) {
  if (rc != MOSQ_ERR_SUCCESS) {
    qDebug() << "MqttClient Connection error";
    this->connectionError();
    // FIXME handle error
    return;
  }
  qDebug() << "MqttClient Connected";
  this->connected();
  mosquitto_subscribe(impl_.get(), nullptr, "#", 2); // FIXME
}

void MqttClient::_on_disconnect(int rc) {
  bool expected = rc == MOSQ_ERR_SUCCESS;
  (void)expected; // FIXME handle unexpected disconnection
  this->disconnected();
}

void MqttClient::_on_publish(int) {
  qDebug() << "MqttClient Published";
}

void MqttClient::_on_message(const mosquitto_message* message) {
  qDebug() << "MqttClient Message of topic \"" << message->topic << "\", size" << message->payloadlen;
  // FIXME
  QString topic{message->topic};
  QByteArray payload{reinterpret_cast<const char*>(message->payload), message->payloadlen};
  this->message(topic, payload);
}

void MqttClient::_on_subscribe(int, int, const int*) {
  qDebug() << "MqttClient Subscribed";
}

void MqttClient::_on_unsubscribe(int) {
  qDebug() << "MqttClient Unsubscribed";
}
