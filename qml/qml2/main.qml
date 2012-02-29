// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import PvLibrary 1.0

Rectangle {
    width: 360
    height: 360
    PvObject {
        id: pv1
        channel: "catest"
    }
    Text {
        id: pv1_display
        color: "green"
        text: pv1.value[0]
        anchors.fill: parent
    }
}
