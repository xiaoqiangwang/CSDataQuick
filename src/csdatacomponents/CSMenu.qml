import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSMenu
    \inqmlmodule CSDataQuick.Components
    \brief Displays process variable's enumerate in drop-down menu

    The Menu is used for ENUM data and is a means for picking the ENUM choices via a menu.
    Note that if the colorMode is alarm, the foreground color, not the background color, is set to the alarm colors.
    The background color should be choosen to contrast with all the alarm colors.

    \qml
    CSMenu {
        source: 'bo'
    }
    \endqml

    \image menu.png
*/

CSControl {
    id: root

    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height - 4, true)

    implicitWidth: 100
    implicitHeight: 20

    ComboBox {
        id: combo_control
        anchors.fill: parent
        enabled: csdata.accessRight & CSData.WriteAccess
        model: csdata.stateStrings
        onModelChanged: {
            if (csdata.value !== undefined)
                currentIndex = csdata.value
        }

        style: ComboBoxStyle {
            padding { top: 2 ; left: 2 ; right: 14 ; bottom:2 }
            label: Text {
                text: combo_control.currentText
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
                font.pixelSize: root.font.size
                font.family: root.font.family
            }
            background: StyledFrame {
                anchors.fill: parent
                StyledFrame {
                    width: 9
                    height: 7
                    shadow: FrameShadow.Raise
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 7
                    opacity: control.enabled ? 0.7 : 0.5
                    color: root.background
                }
                shadow: FrameShadow.Raise
                color: root.background
            }
        }
/*
        delegate: ItemDelegate {
            background: root.background
            text: modelData
        }

        indicator: StyledFrame {
            width: 9
            height: 7
            shadow: FrameShadow.Raise
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 7
            opacity: control.enabled ? 0.7 : 0.5
            color: root.background
        }

        contentItem: Text {
            text: combo_control.currentText
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
            font.pixelSize: root.font.size
            font.family: root.font.family
        }
        background: StyledFrame {
            anchors.fill: parent
            shadow: FrameShadow.Raise
            color: root.background
        }
*/
        onActivated: {
            csdata.setValue(index)
        }
    }
    Connections {
        target: csdata
        onValueChanged: {
            combo_control.currentIndex = csdata.value
        }
    }
}
