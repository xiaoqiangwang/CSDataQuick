import QtQuick 2.0
import PvComponents 1.0


/*!
    \qmltype CaMonitor
    \inqmlmodule PvComponents
    \brief Basic type to all PV monitors

*/

BaseItem {
    /*! foreground color */
    property color background: ColorMap.color4
    /*! background color */
    property color foreground: ColorMap.color14
    /*! qmlproperty enumeration CaGraphics::colorMode
    */
    property int colorMode: ColorMode.Static
    /*! font size */
    property int fontSize: 11
    /*! font family */
    property string fontFamily: fontSize <= 22 ?'Dejavu Sans Mono' : 'Helvetica'

    /*! control pv name */
    property alias channel: pv.channel
    /*! control pv object */
    property var pv: PvObject { id: pv; }

    /*! dynamic attributes */
    property var dynamicAttr: DynamicAttr { id: da }

    property color alarmColor: ColorMap.invalid_alarm

    visible: da.visibility

    Rectangle {
        anchors.fill: parent
        color: background
    }

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
            switch (pv.severity) {
                case 0: // NO_ALARM
                alarmColor = ColorMap.no_alarm
                break;
                case 1: // MINOR_ALARM
                alarmColor = ColorMap.minor_alarm
                break;
                case 2: // MAJOR_ALARM
                alarmColor = ColorMap.major_alarm
                break;
                case 3: // INVALID_ALARM
                alarmColor = ColorMap.invalid_alarm
                break;
            }
        }
    }
}
