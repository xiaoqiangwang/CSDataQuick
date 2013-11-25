import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0


CaControl {
    id: slider
    background: 'darkgray'
    property bool usePVLimits: true
    property alias orientation: slider_control.orientation

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
