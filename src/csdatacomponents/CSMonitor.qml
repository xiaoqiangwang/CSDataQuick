import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSMonitor
    \inqmlmodule CSDataQuick.Components
    \brief Basic type to all PV monitors

*/

BaseItem {
    background: ColorMap.monitors_background
    foreground: ColorMap.foreground
    /*! qmlproperty enumeration CSGraphics::colorMode */
    property int colorMode: ColorMode.Static
    /*! control data source name */
    property alias source: pv.source
    /*! control data object */
    property var pv: CSData { id: pv; }

    /*! dynamic attributes */
    dynamicAttribute: DynamicAttribute {id: da}

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
