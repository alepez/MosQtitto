#include "./lib/MqttClient.hpp"
#include "./lib/Sensor.hpp"
#include <mosquitto.h>

#include <QDebug>
#include <QGuiApplication>
#include <QQuickView>
#include <QSslConfiguration>

int main(int argc, char* argv[]) {
  mosquitto_lib_init();
  // FIXME mosquitto_lib_cleanup must be called on release

  /* Disable SSL Certificate Verification */
  QSslConfiguration sslConf = QSslConfiguration::defaultConfiguration();
  sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
  QSslConfiguration::setDefaultConfiguration(sslConf);

  qmlRegisterType<Sensor>("net.pezzato.myhome", 1, 0, "Sensor");
  qmlRegisterType<MqttClient>("net.pezzato.myhome", 1, 0, "MqttClient");
  qmlRegisterSingletonType<MqttClient>("net.pezzato.myhome", 1, 0, "MqttClientSingleton",
                                       [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
                                         Q_UNUSED(engine);
                                         Q_UNUSED(scriptEngine);
                                         return new MqttClient{};
                                       });

  QGuiApplication app(argc, argv);
  app.setOrganizationName("Alessandro Pezzato");
  app.setOrganizationDomain("pezzato.net");
  app.setApplicationName("MyHome");

  /* Qt Quick View */
  QQuickView view;
  view.setSource(QUrl("qrc:///qml/App.qml"));
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.show();

  auto rc = app.exec();
	mosquitto_lib_cleanup();
	return rc;
}
