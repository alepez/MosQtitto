import QtQuick 2.2
import net.pezzato.mqtt 1.0

Rectangle {
  id: root

  color: '#000000'

  Text {
    id: logViewer
    text: ''
    color: '#ffffff'
  }

  MqttClient {
    id: mqtt

    onMessage: {
      logViewer.text += topic + ' => ' + payload + '\n';
    }
  }

  Component.onCompleted: {
    mqtt.connect("test.mosquitto.org", 1883, 60);
  }
}
