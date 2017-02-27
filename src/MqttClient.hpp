#ifndef MQTTCLIENT_HPP_WFI9XV5R
#define MQTTCLIENT_HPP_WFI9XV5R

#include <QJsonObject>
#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QVariant>
#include <tuple>

struct mosquitto_message;

class MqttClient : public QThread {
  Q_OBJECT
  Q_PROPERTY(QVariantMap options MEMBER options_ NOTIFY optionsChanged);
  Q_PROPERTY(bool isConnected MEMBER isConnected_ NOTIFY isConnectedChanged);

public:
  MqttClient(QObject* parent = nullptr);
  virtual ~MqttClient();

  Q_INVOKABLE void connect();
  Q_INVOKABLE void disconnect();

  Q_INVOKABLE void publish(QString topic, QByteArray payload, int qos = 2);
  Q_INVOKABLE void jsonPublish(QString topic, QJsonObject payload, int qos = 2);
  Q_INVOKABLE void subscribe(QString topic, int qos = 2);

signals:
  void connected();
  void disconnected();
  void message(QString topic, QByteArray payload);
  void jsonMessage(QString topic, QJsonObject payload);

  void optionsChanged();
  void isConnectedChanged();

signals: /* (internal use only) */
  void _connected();
  void _disconnected();
  void _message(QString topic, QByteArray payload);
  void _jsonMessage(QString topic, QJsonObject payload);

public: /* (internal use only) */
  void _onConnect(int);
  void _onDisconnect(int);
  void _onMessage(const mosquitto_message*);

private:
  void run();

  QMutex mutex_;
  QQueue<std::tuple<QString, QByteArray, int>> publishQueue_;
  QQueue<std::tuple<QString, int>> subscribeQueue_;
  QVariantMap options_;
  bool continueRunning_;
  bool isConnected_;
};

#endif /* end of include guard: MQTTCLIENT_HPP_WFI9XV5R */
