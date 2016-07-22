import QtQuick 2.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSSlider
    \inqmlmodule CSDataQuick.Components
    \brief Display a slider to control a PV

    Dragging the Slider handle with Btn1 transmits values.
    The sensitivity with which values can be selected this way depends on the range of the slider.

    The left and right arrow keys are operational when \l direction is Direction.Left or Direction.Right,
    and the up and down arrow keys are operational when \l direction is Direction.Up or Direction.Down.
    The arrow keys only function when the focus is on the slider button or the space on either side of it.

    \qml
    Row {
        spacing: 5
        CSSlider {
            width: 150
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            source: 'catest'
        }
        CSSlider {
            width: 150
            height: 50
            source: 'catest'
            labelStyle: LabelStyle.Limits
        }
    }
    \endqml

    \image slider.png
*/

CSControl {
    id: root
    implicitWidth: 200
    implicitHeight: 50
    /*!
        \qmlproperty enumeration direction
        The incremental direction.
    */
    property int  direction: Direction.Right
    /*! The operation limit and precision */
    limits: Limits {}
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
    /*!
        \qmlproperty enumeration labelStyle
        The decoration mode.

        \list
        \li LabelStyle.Frame - No extra features.
        \li LabelStyle.None - Same as LabelStyle.Frame.
        \li LabelStyle.Outline - Show the limits.
        \li LabelStyle.Limits - Show limits and a box for the value.
        \li LabelStyle.Channel - In addition to LabelStyle.Limits, show the process variable name.
        \endlist
    */
    property int labelStyle: LabelStyle.Frame

    /*! \internal */
    property bool __disconnect: false
    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(
                                     (orientation == Qt.Horizontal ? height / 4 : width / 4), 0)
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

        text: root.source
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignBottom

        font.family: root.font.family
        font.pixelSize: root.font.size

        visible: labelStyle == LabelStyle.Channel
    }

    SliderControl {
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
        precision: limits.prec
        stepSize: root.stepSize
        direction: root.direction

        showRange: labelStyle == LabelStyle.Outline || labelStyle == LabelStyle.Limits ||  labelStyle == LabelStyle.Channel
        showValueText: labelStyle == LabelStyle.Limits ||  labelStyle == LabelStyle.Channel
        valueTextColor: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground

        enabled: pv.accessRight & CSData.WriteAccess

        Connections {
            target: pv
            onConnectionChanged: {
                if (pv.connected) {
                    if (pv.range.isValid()) {
                        limits.loprChannel = pv.range.lower
                        limits.hoprChannel = pv.range.upper
                    }
                    limits.precChannel = pv.precision
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