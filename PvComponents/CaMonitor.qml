import QtQuick 2.0
import PvComponents 1.0

Item {
    property color background: '#bbbbbb'
    property color foreground: '#000000'
    property int colorMode: ColorMode.Static
    property int fontSize: 11
    property string fontFamily: fontSize <= 22 ?'Dejavu Sans Mono' : 'Helvetica'

    // control channel
    property alias channel: pv.channel
    property var pv: PvObject { id: pv; }

    // dynamic attributes
    property var dynamicAttr: DynamicAttr { id: da }

    visible: da.visibility

    Rectangle {
        z: 1
        anchors.fill: parent
        color: 'white'
        visible: !pv.connected
    }

    Rectangle {
        id: tooltip
        width: parent.width
        height: parent.height
        z: 1
        color: 'black'
        Text {
            text: channel
            color: 'green'
        }
        visible: false
    }
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.MiddleButton
        onPressAndHold: {
            tooltip.x = mouse.x
            tooltip.y = mouse.y
            tooltip.visible = true
        }
    }

    Connections {
        target: pv

        onStatusChanged: {
            if (colorMode == ColorMode.Static)
                return
            switch (pv.severity) {
                case 0: // NO_ALARM
                foreground = '#00C000'
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
