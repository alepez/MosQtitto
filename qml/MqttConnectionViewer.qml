import QtQuick 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import net.pezzato.mqtt 1.0

Item {
  id: root

  property string hostname: ""
  property int port: 1883
  property int keepalive: 60
  property string username: ""
  property string password: ""

  ListModel {
    id: incoming
  }

  MessagesViewer {
    anchors { fill: parent; }
    model: incoming
  }

  MqttClient {
    id: mqtt

    options: {
      "hostname": root.hostname,
      "port": root.port,
      "keepalive": root.keepalive,
      "username": root.username,
      "password": root.password
    }

    onConnected: subscribe('#');

    onStringMessage: {
      incoming.insert(0, { topic: topic, payload: payload });
      if (incoming.count > 100) {
        incoming.remove(100);
      }
    }

    /* You can also listen to json message, the payload is a js object */
    // onJsonMessage: {
    //   incoming.append({ topic: topic, payload: JSON.stringify(payload) });
    // }
  }

  Component.onCompleted: mqtt.connect();
}

