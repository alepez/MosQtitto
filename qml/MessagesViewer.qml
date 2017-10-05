import QtQuick 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import net.pezzato.mqtt 1.0

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
