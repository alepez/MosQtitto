#ifndef MQTTCLIENT_HPP_WFI9XV5R
#define MQTTCLIENT_HPP_WFI9XV5R

#include <QObject>
#include <memory>
#include <string>
#include <thread>

struct mosquitto;
struct mosquitto_message;

class MqttClient : public QObject {
  Q_OBJECT

public:
  MqttClient(QObject* parent = nullptr);
  ~MqttClient();

  Q_INVOKABLE void connect(QString hostname, int port, int keepalive);

signals:
  void connected();
  void connectionError();
  void disconnected();
  void published();
  void subscribed();
  void unsubscribed();
  void message(QString topic, QByteArray payload);

public: /* (internal use only) */
  void _on_connect(int);
  void _on_disconnect(int);
  void _on_publish(int);
  void _on_message(const mosquitto_message*);
  void _on_subscribe(int, int, const int*);
  void _on_unsubscribe(int);

private:
  std::shared_ptr<mosquitto> impl_;
  std::shared_ptr<std::thread> thread_;
};

#endif /* end of include guard: MQTTCLIENT_HPP_WFI9XV5R */
