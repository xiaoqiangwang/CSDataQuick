import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0

/*!
    \qmltype CaMenu
    \inqmlmodule CSData.Components
    \brief Displays process variable's enumerate in drop-down menu

    The Menu is used for ENUM process variables and is a means for picking the ENUM choices via a menu.
    Note that if the colorMode is alarm, the foreground color, not the background color, is set to the alarm colors.
    The background color should be choosen to contrast with all the alarm colors.

    \qml
    CaMenu {
        source: 'bo'
    }
    \endqml

    \image menu.png
*/

CaControl {
    id: root

    ComboBox {
        id: combo_control
        property bool __first: true
        anchors.fill: parent
        enabled: pv.accessRight & CSData.WriteAccess

        style: ComboBoxStyle {
            padding { top: 2 ; left: 2 ; right: 14 ; bottom:2 }
            label: Text {
                text: combo_control.currentText
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
                font.pixelSize: root.fontSize
                font.family: root.fontFamily
            }
            background: Frame {
                anchors.fill: parent
                Frame {
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

        onCurrentIndexChanged: {
            if (__first)
                __first = false
            else
                pv.setValue(currentIndex)
        }
    }
    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                combo_control.model = pv.stateStrings
            } else {
                combo_control.model = []
            }
        }
        onValueChanged: {
            combo_control.currentIndex = pv.value
        }
    }
}
