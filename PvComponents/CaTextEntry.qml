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

    Rectangle {
        anchors.fill: parent
        BorderImage {
            source: 'images/button_down.png'
            anchors.leftMargin: -1
            anchors.topMargin: -2
            anchors.rightMargin: 1
            anchors.bottomMargin: 2
            border {left: 3; right: 3; top: 3; bottom: 3;}
            width: parent.width
            height: parent.height
        }
        color: entry.background
    }

    TextInput {
        id: textField
        font.pixelSize: entry.fontSize
        font.family: entry.fontFamily
        color: entry.foreground
        verticalAlignment: TextInput.AlignBottom
        text: formatString(format, pv.value)
        anchors.topMargin: 4
        anchors.bottomMargin: 0
        anchors.leftMargin: 4
        anchors.fill: parent

        onAccepted: {
            pv.value = text
        }
    }

/*
    TextField {
        id: textField
        font.pixelSize: entry.pixelSize
        textColor: entry.foreground
        verticalAlignment: TextInput.AlignBottom
        text: formatString(format, pv.value)
        anchors.topMargin: 2
        anchors.bottomMargin: 0
        anchors.fill: parent
        onAccepted: {
            pv.value = text
        }
        style: TextFieldStyle {
            background: Rectangle {
                //anchors.fill: parent
                color: entry.background
                BorderImage {
                    anchors.fill: parent
                    source: 'images/button_down.png'
                    anchors.leftMargin: 1
                    anchors.topMargin: -2
                    anchors.rightMargin: 1
                    anchors.bottomMargin: 2
                    border {left: 3; right: 3; top: 3; bottom: 3;}
                }
                    BorderImage {
                        anchors.fill: parent
                        source: 'images/focusframe.png'
                        anchors.leftMargin: -1
                        anchors.topMargin: -3
                        anchors.rightMargin: -1
                        anchors.bottomMargin: -1
                        border {left: 3; right: 3; top: 3; bottom: 3;}
                        visible: control.activeFocus
                    }


            }

        }
    }
*/
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
