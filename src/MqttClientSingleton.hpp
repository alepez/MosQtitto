#ifndef MQTTCLIENTSINGLETON_HPP_UMMTD9WI
#define MQTTCLIENTSINGLETON_HPP_UMMTD9WI

#include "MqttClient.hpp"

class MqttClientSingleton {
public:
  static MqttClient* get();
  static void set(MqttClient*);
};

#endif /* end of include guard: MQTTCLIENTSINGLETON_HPP_UMMTD9WI */
