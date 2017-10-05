import QtQuick 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import net.pezzato.mqtt 1.0

ApplicationWindow {
  id: root

  visible: true
  width: 960
  height: 640
  title: "MosQtitto"

  Pane {
    anchors { top: parent.top; left: parent.left; right: parent.right; }
    height: parent.height * 0.5

    MqttConnectionViewer {
      anchors { fill: parent; }

      hostname: "test.mosquitto.org"
      port: 1883
      keepalive: 60
    }
  }

  Pane {
    anchors { bottom: parent.bottom; left: parent.left; right: parent.right; }
    height: parent.height * 0.5

    MqttConnectionViewer {
      anchors { fill: parent; }

      hostname: "test.mosquitto.org"
      port: 1883
      keepalive: 60
    }
  }
}
