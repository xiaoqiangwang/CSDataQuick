import QtQuick 2.0
import PvComponents 1.0

Item {
    // appearance
    property color background: '#73dfff' // '#bbbbbb' is the actual default
    property color foreground: '#000000'
    property int colorMode: ColorMode.Static
    property int fontSize: 12
    property string fontFamily: fontSize <= 22 ? 'Courier' : 'Helvetica'

    // control channel
    property alias channel: pv.channel
    property var pv: PvObject { id: pv; }

    // dynamic attributes
    property var dynamicAttr: DynamicAttr { id: da }

    visible: da.visibility

    // Mask when PV disconnected
    Rectangle {
        z: 1
        anchors.fill: parent
        color: 'white'
        visible: channel != '' && !pv.connected
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.MiddleButton
        onPressAndHold: {
            console.log('TODO: Popup tooltip/context menu')
        }
    }

    Connections {
        target: pv
        onStatusChanged: {
            if (colorMode == ColorMode.Static)
                return
            switch (pv.severity) {
                case 0: // NO_ALARM
                foreground = 'green'
                break;
                case 1: // MINOR_ALARM
                foreground = 'yellow'
                break;
                case 2: // MAJOR_ALARM
                foreground = 'red'
                break;
                case 3: // INVALID_ALARM
                foreground = 'white'
                break;
            }
        }
    }
}
