import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import PvComponents 1.0

/*!
    \qmltype CaBar
    \inqmlmodule PvComponents
    \brief Display a bar that expands or contracts when value changes

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

    ColumnLayout {
        anchors.fill: parent

        Text {
            Layout.fillWidth: true
            Layout.minimumHeight: 10

            id: title
            text: root.channel
            horizontalAlignment: Text.AlignHCenter

            visible: label == LabelStyle.Channel
        }

        StyledBar2 {
            id: bar
            Layout.fillWidth: true
            Layout.fillHeight: true

            foreground: root.foreground
            background: root.background
            minimumValue: limits.lopr
            maximumValue: limits.hopr
            showRange: label == LabelStyle.Outline || label == LabelStyle.Limits || label == LabelStyle.Channel
        }

        Text {
            id: value
            Layout.minimumHeight: 10
            Layout.fillWidth: true

            text: pv.value
            horizontalAlignment: Text.AlignHCenter
            visible: label == LabelStyle.Limits || label == LabelStyle.Channel
        }
    }
    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                if (pv.lodisplim < pv.updisplim) {
                    limits.loprChannel = pv.lodisplim
                    limits.hoprChannel = pv.updisplim
                }
            }
        }
        onValueChanged: {
            bar.value = pv.value
        }
    }
    function formatString(format, value) {
        if (pv.type == PvObject.Enum)
            return pv.strs[value]
        if (pv.type == PvObject.String)
            return value
        if (pv.type == PvObject.Char && value instanceof Array)
            return arrayToString(value)
        var result = Utils.convert(format, value, limits.prec)
        return result
    }
}
