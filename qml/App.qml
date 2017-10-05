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

    MessagesViewer {
      anchors { fill: parent; }
      model: incoming
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
      incoming.insert(0, { topic: topic, payload: payload });
      if (incoming.count > 100) {
        incoming.remove(100);
      }
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
