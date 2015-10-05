import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

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

    /*! control data source name */
    property alias source: pv.source
    /*! control data object */
    property var pv: CSData { id: pv; }

    /*! dynamic attributes */
    property var dynamicAttr: DynamicAttr { id: da }
    /*! color based on the severity of the process variable */
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

    Menu {
        id: contextMenu
        MenuItem {
            text: 'PV Info'
            onTriggered: {
                PvInfoDialog.info = UtilsJS.dumpPvInfo(pv)
                PvInfoDialog.open()
            }
        }
        MenuItem {
            text: 'PV Limits'
            visible: root['limits'] ? true : false
            onTriggered: {
                UtilsJS.popupPvLimitsDialog(root)
            }
        }
    }

    MouseArea {
        z: 1
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.MiddleButton
        onReleased: contextMenu.popup()
    }

    Connections {
        target: pv

        onAlarmChanged: {
            switch (pv.alarm.severity) {
                case CSDataAlarm.NoAlarm: // NO_ALARM
                alarmColor = ColorMap.no_alarm
                break;
                case CSDataAlarm.MinorAlarm: // MINOR_ALARM
                alarmColor = ColorMap.minor_alarm
                break;
                case CSDataAlarm.MajorAlarm: // MAJOR_ALARM
                alarmColor = ColorMap.major_alarm
                break;
                case CSDataAlarm.InvalidAlarm: // INVALID_ALARM
                alarmColor = ColorMap.invalid_alarm
                break;
            }
        }
    }
}
