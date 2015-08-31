import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaSlider
    \inqmlmodule PvComponents
    \brief Display a slider to control a PV

    Dragging the Slider handle with Btn1 transmits values.
    The sensitivity with which values can be selected this way depends on the range of the slider.

    The left and right arrow keys are operational when \l direction is Direction.Left or Direction.Right,
    and the up and down arrow keys are operational when \l direction is Direction.Up or Direction.Down.
    The arrow keys only function when the focus is on the slider button or the space on either side of it.

*/

CaControl {
    id: root
    /*!
        \qmlproperty enumeration direction
        The incremental direction.
    */
    property int  direction: Direction.Right
    /*! The low high operation limit and precision */
    property Limits limits: Limits {}
    /*!
        The amount of vaue to increament or decrement.

        Clicking Btn1 in the space on either side of the slider button increments or decrements
        the value by an amount equal to the \l stepSize.

        Clicking Ctrl-Btn1 in the space on either side increments or decrements
        the value by an amount equal to 10 times \l stepSize

        In addition, the arrow keys increment or decrement the value by an amount equal to \l stepSize,
        and the Ctrl-Arrow keys increment or decrement the value by an amount equal to 10 times \l stepSize.
    */
    property real stepSize: 1.0
    /*! The label style */
    property int label: LabelStyle.Frame
    /*! \internal */
    property bool __disconnect: false
    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height / (orientation == Qt.Horizontal ? 4 : 20), 0)
    /*! \internal */
    readonly property int orientation: (direction == Direction.Left || direction == Direction.Right) ? Qt.Horizontal : Qt.Vertical


    Rectangle {
        anchors.fill: parent
        color: background
    }

    Text {
        id: title

        height: root.font.size
        anchors.top: root.top
        anchors.left: root.left
        z: 1

        text: root.channel
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom

        font.family: root.font.family
        font.pixelSize: root.font.size

        visible: label == LabelStyle.Channel
    }

    Slider2 {
        id: slider

        x: 1
        width: root.width - 2
        y: (orientation == Qt.Horizontal && title.visible) ? title.height : 1
        height: root.height - (orientation == Qt.Horizontal && title.visible ? title.height : 2)

        foreground: root.foreground
        background: root.background

        font.family: root.font.family
        font.pixelSize: root.font.size

        minimumValue: limits.lopr
        maximumValue: limits.hopr
        stepSize: root.stepSize
        direction: root.direction

        showValueText: label == LabelStyle.Limits ||  label == LabelStyle.Channel
        valueTextColor: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground

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
                slider.value = pv.value
                __disconnect = false
            }
        }

        onValueChanged: {
            if(__disconnect) return
            pv.setValue(value)
        }
    }
}
