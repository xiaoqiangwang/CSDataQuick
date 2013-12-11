import QtQuick 2.0
import PvComponents 1.0

Item {
    property color background: '#bbbbbb'
    property color foreground: '#000000'
    property string colorMode: 'static'
    property int fontSize: 11
    property string fontFamily: fontSize <= 22 ?'Dejavu Sans Mono' : 'Helvetica'

    // control channel
    property alias channel: pv.channel
    property var pv: PvObject { id: pv; }

    // dynamic attributes
    property alias visibilityMode: da.visibilityMode
    property alias visibilityCalc: da.visibilityCalc
    property alias channelA: da.channel
    property alias channelB: da.channelB
    property alias channelC: da.channelC
    property alias channelD: da.channelD
    visible: da.visibility
    DynamicAttr { id: da }

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
            if (colorMode == 'static')
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
