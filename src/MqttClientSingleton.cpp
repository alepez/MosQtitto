#include "MqttClientSingleton.hpp"
#include <QDebug>

static MqttClient* instance{nullptr};

MqttClient* MqttClientSingleton::get() {
  return instance;
}

void MqttClientSingleton::set(MqttClient* newInstance) {
  instance = newInstance;
}
