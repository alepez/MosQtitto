#include "MqttClient.hpp"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickView>
#include <mosquitto.h>

int main(int argc, char* argv[]) {
  mosquitto_lib_init();

  qmlRegisterType<MqttClient>("net.pezzato.mqtt", 1, 0, "MqttClient");

  QGuiApplication::setApplicationName("MosQtitto");
  QGuiApplication::setOrganizationName("Alessandro Pezzato");
  QGuiApplication::setOrganizationDomain("pezzato.net");
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  QQuickStyle::setStyle("Material");

  QQmlApplicationEngine engine;
  engine.load(QUrl("qrc:///qml/App.qml"));
  if (engine.rootObjects().isEmpty()) return -1;

  auto rc = app.exec();
  mosquitto_lib_cleanup();
  return rc;
}
