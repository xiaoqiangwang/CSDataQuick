import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0

/*!
    \qmltype CaMenu
    \inqmlmodule PvComponents
    \brief Displays process variable's enumerate in drop-down menu

    The Menu is used for ENUM process variables and is a means for picking the ENUM choices via a menu.
    Note that if the colorMode is alarm, the foreground color, not the background color, is set to the alarm colors.
    The background color should be choosen to contrast with all the alarm colors.

    \image menu.png
*/

CaControl {
    id: root

    ComboBox {
        id: combo_control
        property bool __first: true
        anchors.fill: parent
        enabled: pv.writable

        style: ComboBoxStyle {
            label: Text {
                text: combo_control.currentText
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
                font.pixelSize: root.fontSize
                font.family: root.fontFamily
            }
            background: Rectangle {
                anchors.fill: parent
                BorderImage {
                    anchors.fill: parent
                    source: control.pressed ? 'images/button_up.png' : 'images/button_up.png'
                    border {left: 2; right: 2; top: 2; bottom: 2;}
                    smooth: false
                    width: parent.width
                    height: parent.height

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
                }
                color: root.background
            }
        }

        onCurrentIndexChanged: {
            if (__first)
                __first = false
            else if (pv.writable)
                pv.setValue(currentIndex)
        }
    }
    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                combo_control.model = pv.strs
            } else {
                combo_control.model = []
            }
        }
        onValueChanged: {
            combo_control.currentIndex = pv.value
        }
    }
}
