import QtQuick 2.2
import QtQuick.Controls 2.1
import net.pezzato.mqtt 1.0

ApplicationWindow {
  id: root

  visible: true
  width: 960
  height: 640
  title: "MosQtitto"

  Rectangle {
    color: '#ccffcc'
    anchors { top: parent.top; left: parent.left; right: parent.right; }
    height: parent.height * 0.5

    MqttConnectionViewer {
      anchors { fill: parent; }

      /* If a client id is not defined, it will be randomly generated */
      hostname: "test.mosquitto.org"
      port: 1883
      keepalive: 60
      subscriptions: [ '#' ]
    }
  }

  Rectangle {
    color: '#ccccff'
    anchors { bottom: parent.bottom; left: parent.left; right: parent.right; }
    height: parent.height * 0.5

    MqttConnectionViewer {
      anchors { fill: parent; }

      clientId: "MosQtitto-example"
      hostname: "iot.eclipse.org"
      port: 1883
      keepalive: 60
      subscriptions: [ 'sensor/#' ]
    }
  }
}
