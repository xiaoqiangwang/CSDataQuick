import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaSlider
    \inqmlmodule PvComponents
    \brief Display a slider to control a PV
*/

CaControl {
    id: root
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

    readonly property var font: UtilsJS.getBestFontSize(height / (orientation == Qt.Horizontal ? 4 : 20), 0)

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

        height: root.height - (orientation == Qt.Horizontal && title.visible ? title.height : 0)
        width: root.width
        anchors.top: (orientation == Qt.Horizontal && title.visible) ? title.bottom : root.top

        foreground: root.foreground
        background: root.background

        font.family: root.font.family
        font.pixelSize: root.font.size

        minimumValue: limits.lopr
        maximumValue: limits.hopr
        stepSize: root.stepSize
        direction: root.direction

        showValueText: label == LabelStyle.Limits ||  label == LabelStyle.Channel
        valueTextColor: root.foreground

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
