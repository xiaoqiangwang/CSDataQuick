import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaBar
    \inqmlmodule CSDataQuick.Components
    \brief Display a bar that expands or contracts when value changes

    The Bar Monitor can have several decorations as specified by the \l label.
    It can go up, down, left, or right, as specified by the \l direction property.
    It can start from the edge or from the center, as specified by the \l fillMode property.
    The Bar Monitor with the \l label set to LabelStyle.None can be used to make bar graphs.

    \qml
        CaBar {
            source: 'catest'
            label: LabelStyle.Outline
            direction: Direction.Up
       }
    \endqml

    \image bar.png
*/

CaMonitor {
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
    property int label: LabelStyle.Frame
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
            CaBar {
                source: 'catest'
                label: LabelStyle.Outline
                fillMode: FillMode.FromEdge
            }
            CaBar {
                source: 'catest'
                label: LabelStyle.Outline
                fillMode: FillMode.FromCenter
            }
        }
        \endqml

        \image bar-fillMode.png
    */
    property alias fillMode: bar.fillMode
    /*! Operation limits range and precision */
    property Limits limits : Limits{}

    /*!
        \internal
        Determine the font based item total height, indepent of the orientation.
    */
    readonly property var font: UtilsJS.getBestFontSize(height / 8, 0)

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

        visible: label == LabelStyle.Channel
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

        showRange: label == LabelStyle.Outline || label == LabelStyle.Limits || label == LabelStyle.Channel
        showFrame: label != LabelStyle.None

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

        visible: label == LabelStyle.Limits ||  label == LabelStyle.Channel

        Rectangle {
            anchors.fill: parent
            color: 'white'
            z: -1
        }
    }

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
            bar.value = pv.value
        }
    }
}
