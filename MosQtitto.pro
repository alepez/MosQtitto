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
  ANDROID_EXTRA_LIBS += ~/github/eclipse/mosquitto/android/lib/libmosquitto.so
  INCLUDEPATH += ~/github/eclipse/mosquitto/lib
  LIBS += -L~/github/eclipse/mosquitto/android/lib

  DISTFILES += android/AndroidManifest.xml
  DISTFILES += android/gradle/wrapper/gradle-wrapper.jar
  DISTFILES += android/gradlew
  DISTFILES += android/res/values/libs.xml
  DISTFILES += android/build.gradle
  DISTFILES += android/gradle/wrapper/gradle-wrapper.properties
  DISTFILES += android/gradlew.bat
}
