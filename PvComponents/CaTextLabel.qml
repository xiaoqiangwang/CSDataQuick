import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

CaMonitor {
    id: label
    property alias align: label_control.horizontalAlignment
    property int format: TextFormat.Decimal
    property bool withUnits: false
    // limits
    property int precision: 2
    property string limits: ''

    Rectangle {
        id: panel
        color: label.background
        anchors.fill: parent
        Text {
            id: label_control
            font.pixelSize: label.fontSize
            font.family: label.fontFamily
            text: formatString(format, pv.value)
            color: label.foreground
            anchors.left: parent.left
            Connections {
                target: pv
                onConnectionChanged: {
                    units.text = pv.units;
                }
            }
        }
        Text {
            id: units
            font.pixelSize: label.fontSize
            font.family: label.fontFamily
            color: label.foreground
            anchors.left: label_control.right
            anchors.right: parent.right
            verticalAlignment: Text.AlignVCenter
            visible: withUnits
        }
    }

    function formatString(format, value) {
        var result = value
        if (value instanceof Array)
            return value
        if (format == TextFormat.Decimal)
            result = Number(value).toFixed(precision)
        else if (format === TextFormat.Exponential)
            result = Number(value).toExponential(precision)
        else if (format === TextFormat.Hexadecimal) {
            result = '0x' + Number(value).toFixed(0).toString(16)
        } else if (format == TextFormat.Octal)
            result = '0' + Number(value).toString(8)
        else
            console.log('unsupported format')
        return result
    }
}

