import QtQuick 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import net.pezzato.mqtt 1.0

Rectangle {
  id: root

  color: '#000000'

  property var incoming: []

  ListView {
    anchors { fill: parent; }
    model: incoming
    delegate: Rectangle {
      color: '#000000'
      anchors { left: parent.left; right: parent.right; }
      height: childrenRect.height
      Column {
        anchors { left: parent.left; right: parent.right; }
        Text {
          text: modelData.topic
          color: '#cccccc'
        }
        Text {
          text: modelData.payload
          color: '#ffffff'
        }
        Rectangle {
          anchors { left: parent.left; right: parent.right; }
          height: 1
          color: '#ffffff'
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

    onMessage: {
      // console.log('message', topic);
      incoming.push({ topic: topic, payload: payload });
      incomingChanged();
    }
  }

  Component.onCompleted: {
    mqtt.connect();
  }
}
