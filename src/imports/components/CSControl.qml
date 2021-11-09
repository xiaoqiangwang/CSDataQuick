import QtQml 2.0
import QtQuick 2.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Compat 1.0 as Compat
import "utils.js" as UtilsJS

/*!
    \qmltype CSControl
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.bases
    \brief Basic type to all controls.

    CSControl is the base type for all control types.
    It defines all the attributes that are common across controls.


*/
BaseItem {
    id: root

    /*! This property indicates how foreground color changes. \sa ColorMode, */
    property int colorMode: ColorMode.Static
    /*! This property inidicates where the alarmColor is applied. \sa AlarmMode

        The alarmColor can be applied to foreground (default) or background.
    */
    property int alarmMode: AlarmMode.Foreground
    /*! This property holds the data source string */
    property string source
    /*! This property holds the color based on the severity of the associated CSData.

        It is one of the ColorMap.invalid_alarm, ColorMap.minor_alarm, ColorMap.major_alarm and ColorMap.no_alarm.
    */
    property color alarmColor: ColorMap.invalid_alarm
    /*! \internal */
    property var csdata: CSData {
        source: root.source
    }
    /*! \internal */
    property var contextMenu: Compat.Menu {
        Compat.MenuItem {
            text: 'Data Info'
            onTriggered: {
                if (typeof DataInfoDialog.parent !== "undefined")
                    DataInfoDialog.parent = root
                DataInfoDialog.info = UtilsJS.dumpCSData(csdata)
                DataInfoDialog.open()
            }
        }
        Compat.MenuItem {
            text: 'Data Limits'
            onTriggered: {
                UtilsJS.popupDataLimitsDialog(root)
            }
        }
        Compat.MenuItem {
            text: 'Copy Source'
            onTriggered: {
                Utils.copyToClipboard(root.source)
            }
        }
    }

    background: ColorMap.controls_background
    foreground: ColorMap.foreground
    visible: Utils.inPuppet || dynamicAttribute.visibility

    // Mask when data is disconnected
    Rectangle {
        z: 1
        anchors.fill: parent
        color: 'white'
        visible: !csdata.connected
    }

    MouseArea {
        z: 1
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.MiddleButton
        cursorShape: csdata.accessRight & CSData.WriteAccess ? Qt.ArrowCursor : Qt.ForbiddenCursor
        onReleased: contextMenu.popup()
    }

    Connections {
        target: csdata

        onAlarmChanged: {
            switch (csdata.alarm.severity) {
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
