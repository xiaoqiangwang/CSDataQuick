import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0

/*!
    \qmltype CaSlider
    \inqmlmodule PvComponents
    \brief Display a slider to control a PV
*/

CaControl {
    id: slider
    /*! The incremental direction */
    property int  direction: 0 // right up left down
    /*! The low high operation limit and precision */
    property Limits limits: Limits {}
    /*! The step size */
    property real stepSize: 1.0
    /*! The label style */
    property int label: LabelStyle.Frame
    /*! \internal */
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
        stepSize: slider.stepSize

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
