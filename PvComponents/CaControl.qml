import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

/*!
    \qmltype CaControl
    \inqmlmodule PvComponents
    \brief Basic type to all PV controls

    CaControl is the base type for all control types.
    It defines all the attributes that are common across PV controls.


*/
BaseItem {
    // appearance
    /*! The foreground color. */
    property color background: ColorMap.color51 // '#bbbbbb' is the actual default

    /*! The background color. */
    property color foreground: ColorMap.color14

    /*! qmlproperty enumeration CaControl::colorMode
    */
    property int colorMode: ColorMode.Static

    /*! The font size for text display */
    property int fontSize: 12
    /*! The font family for text display */
    property string fontFamily: fontSize <= 22 ? 'Courier' : 'Helvetica'

    /*! \qmlproperty string CaControl::channel
        The PV channel name.
    */
    property alias channel: pv.channel

    /*! internal */
    property var pv: PvObject { id: pv; }

    /*! dynamic attributes */
    property DynamicAttr dynamicAttr: DynamicAttr { id: da }
    /*! color based on the severity of the process variable */
    property color alarmColor: ColorMap.invalid_alarm

    visible: da.visibility

    // Mask when PV disconnected
    Rectangle {
        z: 1
        anchors.fill: parent
        color: 'white'
        visible: channel != '' && !pv.connected
    }

    MouseArea {
        z: 1
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.MiddleButton
        cursorShape: pv.writable ? Qt.ArrowCursor : Qt.ForbiddenCursor
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
