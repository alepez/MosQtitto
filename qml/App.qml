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

  ListModel {
    id: incoming
  }

  Pane {
    anchors { fill: parent; }
    ListView {
      anchors { fill: parent; }
      model: incoming
      delegate: Column {
        anchors { left: parent.left; right: parent.right; }
        Text {
          text: topic
          color: '#666666'
          font.pixelSize: 10
        }
        Text {
          text: payload.toString()
          color: '#000000'
          font.pixelSize: 15
        }
        Rectangle {
          anchors { left: parent.left; right: parent.right; }
          height: 1
          color: '#000000'
        }
      }
    }
  }

  MqttClient {
    id: mqtt

    options: {
      "hostname": "test.mosquitto.org",
      "port": 1883,
      "keepalive": 60,
      // "username": "",
      // "password": "",
    }

    onConnected: {
      subscribe('#');
    }

    onStringMessage: {
      incoming.append({ topic: topic, payload: payload });
    }

    // onJsonMessage: {
    //   incoming.append({ topic: topic, payload: JSON.stringify(payload) });
    // }
  }

  Component.onCompleted: {
    console.log('component completed');
    mqtt.connect();
  }
}
