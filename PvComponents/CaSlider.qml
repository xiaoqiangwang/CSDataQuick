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
    property int  direction: Direction.Right
    /*! The low high operation limit and precision */
    property Limits limits: Limits {}
    /*! The step size */
    property real stepSize: 1.0
    /*! The label style */
    property int label: LabelStyle.Frame
    /*! \internal */
    property bool __disconnect: false

    readonly property int orientation: (direction == Direction.Left || direction == Direction.Right) ? Qt.Horizontal : Qt.Vertical

    Slider {
        id: slider_control
        width: orientation == Qt.Horizontal ? slider.width : slider.height
        height: orientation == Qt.Horizontal ? slider.height : slider.width

        transform: Rotation {
            origin.x: (direction == Direction.Right || direction == Direction.Left)
                      ? width / 2
                      : (direction == Direction.Up) ? height / 2 : width / 2
            origin.y: (direction == Direction.Right || direction == Direction.Left)
                      ? height / 2
                      : (direction == Direction.Up ? height / 2 : width / 2)
            angle: direction == Direction.Right ? 0 : direction == Direction.Up ? -90 : direction == Direction.Left ? -180 : -270
        }

        minimumValue: limits.lopr
        maximumValue: limits.hopr
        stepSize: slider.stepSize
        updateValueWhileDragging: true

        style:SliderStyle {
            groove: Rectangle {
                implicitWidth: 200
                implicitHeight: 10
                color: slider.background
                radius: 2
            }
            handle: Rectangle {
                implicitHeight: 20
                implicitWidth: 8
                color: Qt.darker(slider.background, control.pressed ? 1.3 : 1.1)
                radius: 1
                Rectangle {
                    width: 2
                    height: parent.height * 2 / 3
                    anchors.centerIn: parent
                    color: Qt.darker(slider.background, 2)

                }
            }
        }

        Connections {
            target: pv
            onConnectionChanged: {
                if (pv.connected) {
                    if (pv.lodisplim < pv.updisplim) {
                        limits.lopr = pv.lodisplim
                        limits.hopr = pv.updisplim
                        limits.prec = pv.prec
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
