import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaBar
    \inqmlmodule PvComponents
    \brief Display a bar that expands or contracts when value changes

    The Bar Monitor can have several decorations as specified by the \l label.
    It can go up, down, left, or right, as specified by the \l direction property.
    It can start from the edge or from the center, as specified by the \l fillMode property.
    The Bar Monitor with the \l label set to LabelStyle.None can be used to make bar graphs.
*/

CaMonitor {
    id: root
    /*!
        \qmlproperty enumeration label
        The decoration mode.
    */
    property int label: LabelStyle.Frame
    /*!
        \qmlproperty enumeration direction
        The expanding direction.
    */
    property alias direction: bar.direction
    /*!
        \qmlproperty enumeration fillMode
        Either Edge or Center.
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

        text: root.channel
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
                if (pv.lodisplim < pv.updisplim) {
                    limits.loprChannel = pv.lodisplim
                    limits.hoprChannel = pv.updisplim
                    limits.precChannel = pv.prec
                }
            }
        }
        onValueChanged: {
            bar.value = pv.value
        }
    }
}
