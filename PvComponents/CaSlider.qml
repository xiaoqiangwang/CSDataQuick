import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0


CaControl {
    id: slider
    property alias minimumValue: slider_control.minimumValue
    property alias maximumValue: slider_control.maximumValue
    property alias orientation: slider_control.orientation
    property alias stepSize: slider_control.stepSize
    property bool usePVLimits: true

    Slider {
        id: slider_control
        anchors.fill: parent
        style:SliderStyle {
            groove: Rectangle {
                implicitWidth: 200
                implicitHeight: 8
                color: slider.background
                radius: 8
            }
            handle: Rectangle {
                implicitHeight: 20
                implicitWidth: 10
                color: Qt.darker(slider.background, control.pressed ? 1.3 : 1.1)
                radius: 5
            }
        }

        Connections {
            target: pv
            onConnectionChanged: {
                if (pv.connected && usePVLimits) {
                    if (pv.lodisplim < pv.updisplim) {
                        minimumValue = pv.lodisplim
                        maximumValue = pv.updisplim
                    }
                }
            }
            onValueChanged: {
                slider_control.value = pv.value
            }
        }
        onValueChanged: pv.setValue(value)
    }
}
