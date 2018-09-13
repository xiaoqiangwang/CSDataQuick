import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSControl
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.bases
    \brief Basic type to all controls

    CSControl is the base type for all control types.
    It defines all the attributes that are common across controls.


*/
BaseItem {
    id: root

    /*! This property indicates how foreground color changes. \sa ColorMode, */
    property int colorMode: ColorMode.Static
    /*! This property holds the data source string */
    property string source
    /*! This property holds the color based on the severity of the associated CSData.

        It is one of the ColorMap.invalid_alarm, ColorMap.minor_alarm, ColorMap.major_alarm and ColorMap.no_alarm.
    */
    property color alarmColor: ColorMap.invalid_alarm
    /*! \internal */
    property alias dynamicAttribute_visibilityMode: da.visibilityMode
    /*! \internal */
    property alias dynamicAttribute_visibilityCalc: da.visibilityCalc
    /*! \internal */
    property alias dynamicAttribute_channel: da.channel
    /*! \internal */
    property alias dynamicAttribute_channelB: da.channelB
    /*! \internal */
    property alias dynamicAttribute_channelC: da.channelC
    /*! \internal */
    property alias dynamicAttribute_channelD: da.channelD
    /*! \internal */
    property var csdata: CSData {
        source: root.source
    }
    /*! \internal */
    property Menu contextMenu: Menu {
        MenuItem {
            text: 'Data Info'
            onTriggered: {
                DataInfoDialog.info = UtilsJS.dumpCSData(csdata)
                DataInfoDialog.open()
            }
        }
        MenuItem {
            text: 'Data Limits'
            visible: root['limits'] ? true : false
            onTriggered: {
                UtilsJS.popupDataLimitsDialog(root)
            }
        }
        MenuItem {
            text: 'Copy Source'
            onTriggered: {
                Utils.copyToClipboard(root.source)
            }
        }
    }

    background: ColorMap.controls_background
    foreground: ColorMap.foreground
    dynamicAttribute: DynamicAttribute {id: da}
    visible: Utils.inPuppet || da.visibility

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
