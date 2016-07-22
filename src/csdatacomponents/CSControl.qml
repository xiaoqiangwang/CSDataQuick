import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSControl
    \inqmlmodule CSDataQuick.Components
    \brief Basic type to all PV controls

    CSControl is the base type for all control types.
    It defines all the attributes that are common across PV controls.


*/
BaseItem {
    id: root

    background: ColorMap.controls_background
    foreground: ColorMap.foreground

    /*! qmlproperty enumeration CSControl::colorMode */
    property int colorMode: ColorMode.Static

    /*! \qmlproperty string CSControl::source
        The data source url.
    */
    property string source

    /*! internal */
    property var pv: CSData {
        source: root.source
    }

    /*! dynamic attributes */
    dynamicAttribute: DynamicAttribute {id: da}

    /*! color based on the severity of the process variable */
    property color alarmColor: ColorMap.invalid_alarm

    visible: da.visibility

    // Mask when PV disconnected
    Rectangle {
        z: 1
        anchors.fill: parent
        color: 'white'
        visible: pv.source && !pv.connected
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
        cursorShape: pv.accessRight & CSData.WriteAccess ? Qt.ArrowCursor : Qt.ForbiddenCursor
        onReleased: contextMenu.popup()
    }

    Connections {
        target: pv

        onAlarmChanged: {
            switch (pv.alarm.severity) {
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
