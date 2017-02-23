#include "./MqttClient.hpp"
#include <mosquitto.h>

#include <QGuiApplication>
#include <QQuickView>

int main(int argc, char* argv[]) {
  mosquitto_lib_init();

  qmlRegisterType<MqttClient>("net.pezzato.mqtt", 1, 0, "MqttClient");

  QGuiApplication app(argc, argv);
  app.setOrganizationName("Alessandro Pezzato");
  app.setOrganizationDomain("pezzato.net");
  app.setApplicationName("MosQtitto");

  /* Qt Quick View */
  QQuickView view;
  view.setSource(QUrl("qrc:///qml/App.qml"));
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.show();

  auto rc = app.exec();
  mosquitto_lib_cleanup();
  return rc;
}
