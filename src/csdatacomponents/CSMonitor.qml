import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSMonitor
    \inqmlmodule CSDataQuick.Components
    \brief Basic type to all monitors

*/

BaseItem {
    /*! This property indicates how foreground color changes. \sa ColorMode, */
    property int colorMode: ColorMode.Static
    /*! This property holds the data source string */
    property alias source: pv.source
    /*! This property holds the color based on the severity of the associated CSData.

        It is one of the ColorMap.invalid_alarm, ColorMap.minor_alarm, ColorMap.major_alarm and ColorMap.no_alarm.
    */
    property color alarmColor: ColorMap.invalid_alarm
    /*! \internal  */
    property var pv: CSData { id: pv; }
    /*! \internal */
    property Menu contextMenu: Menu {
        MenuItem {
            text: 'Data Info'
            onTriggered: {
                PvInfoDialog.info = UtilsJS.dumpPvInfo(pv)
                PvInfoDialog.open()
            }
        }
        MenuItem {
            text: 'Data Limits'
            visible: root['limits'] ? true : false
            onTriggered: {
                UtilsJS.popupPvLimitsDialog(root)
            }
        }
        MenuItem {
            text: 'Copy Source'
            onTriggered: {
                Utils.copyToClipboard(root.source)
            }
        }
    }

    background: ColorMap.monitors_background
    foreground: ColorMap.foreground
    dynamicAttribute: DynamicAttribute {id: da}
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
