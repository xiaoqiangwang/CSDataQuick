import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSBar
    \inqmlmodule CSDataQuick.Components
    \brief Display a bar that expands or contracts when value changes

    The Bar Monitor can have several decorations as specified by the \l labelStyle.
    It can go up, down, left, or right, as specified by the \l direction property.
    It can start from the edge or from the center, as specified by the \l fillMode property.
    The Bar Monitor with the \l labelStyle set to LabelStyle.None can be used to make bar graphs.

    \qml
        CSBar {
            source: 'catest'
            labelStyle: LabelStyle.Outline
            direction: Direction.Up
       }
    \endqml

    \image bar.png
*/

CSMonitor {
    id: root
    /*!
        \qmlproperty enumeration label
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
    /*!
        \qmlproperty enumeration direction
        Indicate the maximumValue position.

        \list
        \li Direction.Up
        \li Direction.Right
        \li Direction.Down
        \li Direction.Left
        \endlist

        By Direction.Up, the minimumValue is at the bottom and maximumValue at the top. And by Direction.Right,
        the minimumValue is at the left and maximumValue at the right. The other two reverse the direction.
    */
    property alias direction: bar.direction
    /*!
        \qmlproperty enumeration fillMode
        Set the bar start postion.
        \list
        \li FillMode.FromEdge - the default
        \li FillMode.FromCenter
        \endlist

        If the fill mode is FillMode.Edge, the bar starts from where the minimum value is.
        And the length, in percentage, is calculated by (value - minimumValue) / (maximumValue - minimumValue).

        In FillMode.Center mode, the bar starts from where the middle point is, i.e. (maximumValue - minimumValue) / 2.
        And the length, in percentage, is calculated by (value - middleValue) / (maximumValue - minimumValue) / 2.
        If the value is less than middle point, the bar is towards minimum value.

        \qml
        Row {
            CSBar {
                source: 'catest'
                labelStyle: LabelStyle.Outline
                fillMode: FillMode.FromEdge
            }
            CSBar {
                source: 'catest'
                labelStyle: LabelStyle.Outline
                fillMode: FillMode.FromCenter
            }
        }
        \endqml

        \image bar-fillMode.png
    */
    property alias fillMode: bar.fillMode

    /*!
        \internal
        Determine the font based item total height, indepent of the orientation.
    */
    readonly property var font: UtilsJS.getBestFontSize(height / 10, true)

    limits: Limits {
        precChannel: pv.precision
    }
    implicitWidth: (direction == Direction.Left || direction == Direction.Right) ? 200 : 50
    implicitHeight:  (direction == Direction.Left || direction == Direction.Right) ? 50 : 200

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

    StyledBar {
        id: bar

        height: root.height - (title.visible ? title.height : 0) - (value_text.visible ? value_text.height : 0)
        width: root.width
        anchors.top: title.visible ? title.bottom : root.top

        foreground: root.foreground
        background: root.background
        indicatorColor: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground

        minimumValue: limits.lopr
        maximumValue: limits.hopr
        precision: limits.prec

        showRange: labelStyle == LabelStyle.Outline || labelStyle == LabelStyle.Limits || labelStyle == LabelStyle.Channel
        showFrame: labelStyle != LabelStyle.None

        font.family: root.font.family
        font.pixelSize: root.font.size
    }

    Text {
        id: value_text

        height: root.font.size
        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter

        text: Utils.convert(TextFormat.Decimal, pv.value, limits.prec)
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
        target: pv
        onRangeChanged: {
            if (pv.range.isValid()) {
                limits.loprChannel = pv.range.lower
                limits.hoprChannel = pv.range.upper
            }
        }
        onValueChanged: {
            bar.value = pv.value
        }
    }
}
