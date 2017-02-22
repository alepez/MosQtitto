import QtQuick 2.2
import net.pezzato.mqtt 1.0

Rectangle {
  id: root

  color: '#000000'

  property var incoming: []

  Text {
    id: logViewer
    text: format(root.incoming)
    color: '#ffffff'
    font.pixelSize: 20
  }

  MqttClient {
    id: mqtt

    onMessage: {
      incoming.push({t:topic, p: payload});
      incomingChanged();
    }
  }

  function format(messages) {
    var maxMessages = 100;
    var txt = '';
    var end = incoming.length;
    var begin = Math.max(end - 100, 0);
    var i;
    for (i = end - 1; i >= begin; --i) {
      txt += messages[i].p + '\n';
    }
    return txt;
  }

  Component.onCompleted: {
    mqtt.connect("test.mosquitto.org", 1883, 60);
  }
}