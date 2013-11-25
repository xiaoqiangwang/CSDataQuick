import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0

CaControl {
    id: entry
    // text format
    property alias align: textField.horizontalAlignment
    property int format: TextFormat.Decimal
    // limits
    property string precision: '2'
    property string limits: ''

    TextField {
        id: textField
        textColor: entry.foreground
        style: TextFieldStyle {
            background: Rectangle {
                border.width: 1
                color: entry.background
            }
        }
        text: formatString(format, pv.value)
        anchors.fill: parent
        onAccepted: {
            pv.value = text
        }
    }


    function formatString(format, value) {
        var result = value
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
