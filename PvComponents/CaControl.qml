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

    Menu {
        id: contextMenu
        MenuItem {
            text: 'PV Info'
            onTriggered: {
                PvInfoDialog.info = dumpPvInfo()
                PvInfoDialog.open()
            }
        }
    }

    MouseArea {
        z: 1
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.MiddleButton
        cursorShape: pv.writable ? Qt.ArrowCursor : Qt.ForbiddenCursor
        onReleased: contextMenu.popup()
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

    function dumpPvInfo() {
        var date = new Date()
        var text
        text = '\tPV Infomation\n\n'
        text += date + '\n\n'
        text += pv.channel + '\n'
        text += '======================================\n'
        text += 'TYPE: %1\n'.arg(pv.type)
        text += 'COUNT: %1\n'.arg(pv.count)
        text += 'ACCESS: '
        if (pv.readable)
            text += 'R'
        if (pv.writable)
            text += 'W'
        text += '\n'
        text += 'HOST: %1\n'.arg(pv.host)
        text += 'VALUE: %1\n'.arg(pv.value)
        text += 'STAMP: %1\n'.arg(pv.stamp)
        // PV type specific information
        text += '\n'
        if (pv.type == PvObject.Float || pv.type == PvObject.Double)
            text += 'PRECISION: %1\n'.arg(pv.prec)
        if (pv.loctrllim != pv.upctrllim)
            text += 'HOPR: %1  LOPR %2\n'.arg(pv.upctrllim).arg(pv.loctrllim)

        if (pv.nostr != 0)
            text += 'STATES: %1\n'.arg(pv.nostr)
        for(var i=0; i<pv.strs.length; i++) {
            text += 'STATE %1: %2\n'.arg(i).arg(pv.strs[i])
        }
        return text
    }
}
