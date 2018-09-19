import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSIndicator
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.monitors
    \brief The Indicator displays the value on a scale.

    The Indicator can have several decorations as specified by the \l labelStyle.
    It can go up, down, left, or right, as specified by the \l direction property.
    It is very much like \l CSBar monitor, except that the value is represented by
    an indicator.

    \qml
    CSIndicator {
        source: 'catest'
        labelStyle: LabelStyle.Outline
        colorMode: ColorMode.Alarm
    }
    \endqml

    \image indicator.png

    \sa CSBar
*/

CSMonitor {
    id: root
    /*!
        \qmlproperty enumeration label
        This property holds the decoration mode.

        \list
        \li LabelStyle.Frame - No extra features.
        \li LabelStyle.None - Same as LabelStyle.Frame.
        \li LabelStyle.Outline - Show the limits.
        \li LabelStyle.Limits - Show limits and a box for the value.
        \li LabelStyle.Channel - In addition to LabelStyle.Limits, show the process variable name.
        \endlist
    */
    property int labelStyle: LabelStyle.Frame
    /*!
        \qmlproperty enumeration direction
        This property indicates the maximumValue position.

        \list
        \li Direction.Up
        \li Direction.Right
        \li Direction.Down
        \li Direction.Left
        \endlist

        By Direction.Up, the minimumValue is at the bottom and maximumValue at the top. And by Direction.Right,
        the minimumValue is at the left and maximumValue at the right. The other two reverse the direction.
    */
    property int direction: Direction.Right
    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height / 8, 0)

    implicitWidth: 200
    implicitHeight: 50

    /*!
        Operation limits range and precision
    */
    limits.precChannel: csdata.precision

    Text {
        id: title

        height: root.font.size
        anchors.top: root.top
        anchors.horizontalCenter: root.horizontalCenter

        text: root.source
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom

        font.family: root.font.family
        font.pixelSize: root.font.size

        visible: labelStyle == LabelStyle.Channel
    }

    Indicator {
        id: indicator

        height: root.height - (title.visible ? title.height : 0) - (value_text.visible ? value_text.height : 0)
        width: root.width
        anchors.top: title.visible ? title.bottom : root.top

        direction: root.direction

        foreground: root.foreground
        background: root.background
        indicatorColor:(colorMode == ColorMode.Alarm && !Utils.inPuppet) ? root.alarmColor : root.foreground

        minimumValue: limits.lopr
        maximumValue: limits.hopr
        precision: limits.prec

        showRange: labelStyle == LabelStyle.Outline || labelStyle == LabelStyle.Limits || labelStyle == LabelStyle.Channel

        font.family: root.font.family
        font.pixelSize: root.font.size
    }

    Text {
        id: value_text

        height: root.font.size
        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter

        text: Utils.convert(TextFormat.Decimal, csdata.value, limits.prec)
        horizontalAlignment: Text.AlignHCenter

        font.family: root.font.family
        font.pixelSize: root.font.size

        visible: labelStyle == LabelStyle.Limits ||  labelStyle == LabelStyle.Channel

        Rectangle {
            anchors.fill: parent
            color: 'white'
            z: -1
        }
    }

    Connections {
        target: csdata
        onRangeChanged: {
            if (csdata.range.isValid()) {
                limits.loprChannel = csdata.range.lower
                limits.hoprChannel = csdata.range.upper
            }
        }
        onValueChanged: {
            indicator.value = csdata.value
        }
    }
}
