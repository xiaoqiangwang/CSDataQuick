import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0


CaControl {
    id: slider
    property alias minimumValue: slider_control.minimumValue
    property alias maximumValue: slider_control.maximumValue
    property int  direction: 0 // right up left down
    property alias stepSize: slider_control.stepSize
    property Limits limits: Limits {}

    property bool __disconnect: false
    Slider {
        id: slider_control
        width: (direction == 0 || direction == 2) ? slider.width : slider.height
        height: (direction == 0 || direction == 2) ? slider.height : slider.width

        transform: Rotation {
            origin.x: (direction == 0 || direction == 2) ? width / 2 : direction == 1 ? height / 2 : width / 2
            origin.y: (direction == 0 || direction == 2) ? height / 2 :direction == 1 ? height / 2 : width / 2
            angle: direction == 0 ? 0 : direction == 1 ? -90 : direction == 2 ? -180 : -270
        }

        //anchors.fill: parent
        minimumValue: limits.lopr
        maximumValue: limits.hopr

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
                if (pv.connected) {
                    if (pv.lodisplim < pv.updisplim) {
                        limits.lopr = pv.lodisplim
                        limits.hopr = pv.updisplim
                    }
                }
            }
            onValueChanged: {
                __disconnect = true
                slider_control.value = pv.value
                __disconnect = false
            }
        }

        onValueChanged: {
            if(__disconnect) return
            pv.setValue(value)
        }
    }
}
