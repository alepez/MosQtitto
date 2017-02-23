TEMPLATE = app
QT += qml quick
RESOURCES += resources.qrc
CONFIG += c++11

SOURCES += src/main.cpp
SOURCES += src/MqttClient.cpp

HEADERS += src/MqttClient.hpp

LIBS += -lmosquitto

android: {
  QT += androidextras

  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

  # TODO: Change paths
  ANDROID_EXTRA_LIBS += $$(HOME)/github/eclipse/mosquitto/_build_android/lib/libmosquitto.so
  INCLUDEPATH += $$(HOME)/github/eclipse/mosquitto/lib
  LIBS += -L$$(HOME)/github/eclipse/mosquitto/_build_android/lib

  DISTFILES += android/AndroidManifest.xml
  DISTFILES += android/gradle/wrapper/gradle-wrapper.jar
  DISTFILES += android/gradlew
  DISTFILES += android/res/values/libs.xml
  DISTFILES += android/build.gradle
  DISTFILES += android/gradle/wrapper/gradle-wrapper.properties
  DISTFILES += android/gradlew.bat
}
