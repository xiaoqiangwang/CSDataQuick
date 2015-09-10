import QtQuick 2.0

import PvComponents 1.0

import "utils.js" as UtilsJS
Item {
    id: root
    property var value
    property var newValue
    property string format: ''
    property real hilim: 1.0
    property real lolim: 0.0
    property int prec: 0
    property color foreground: 'black'
    property color background: 'white'
    property alias font: hiddenText.font

    Text {
        id: hiddenText
    }

    QtObject {
        id: d
        property string undef
        property string format: root.format == '' ? calculateFormat(hilim, lolim, prec) : root.format
        property int width: 1
        property int decimal: 0
        property string value: convert(root.value)
        property var order: []
        onFormatChanged: {
            parseFormat()
        }
    }

    function calculateFormat(high, low, precision) {
        var max = Math.max(Math.abs(high), Math.abs(low))
        var width
        if (max >= 1.0) {
            width = Math.floor((Math.log(max)/Math.LN10)) + 3 + precision
        } else
            width = 2 + precision
        var format = '% %1.%2f'.arg(width).arg(precision)
        return format
    }

    function parseFormat() {
        if (d.format.charAt(0) != '%' || d.format.charAt(d.format.length - 1) != 'f')
            return
        var f = d.format.slice(1, -1).split('.')
        var width = Number(f[0])
        var decimal = Number(f[1])

        /* string pattern if number does not fit the width */
        var undef = ''
        if (decimal > 0) {
            undef += '.'
            for (var i = 0; i < decimal; i++)
                undef += '*'
        }
        var tmp = undef.length
        for (var i = 0; i < width - tmp; i++)
            undef = '*' + undef

        /* order pattern */
        var order = []
        if (decimal == 0) {
            for (var i=width -1 ; i>=0; i--)
                order.push(i)
        } else {
            for (var i = width - decimal - 2; i >= 0; i--)
                order.push(i)
            order.push(Number.NaN)
            for (var i = 0; i < decimal; i++)
                order.push(-(i+1))
        }
        d.order = order
        d.width = width
        d.decimal = decimal
        d.undef = undef
    }

    function convert(number) {
        var res =  Utils.format(d.format, number);
        if (res.length > d.width) {
            res = d.undef
        }
        return res
    }

    function getDigit(index) {
        /* during format change, index can overflow */
        if (index >= d.value.length)
            return ' '
        else
            return d.value[index]
    }

    Rectangle {
        id: panel
        anchors.fill: parent
        color: root.background
        BorderImage {
            source: 'images/button_up.png'
            anchors.fill: parent
            border {left: 3; right: 3; top: 3; bottom: 3;}
            horizontalTileMode: BorderImage.Stretch
            verticalTileMode: BorderImage.Stretch
        }
    }

    Row {
        id: wheel
        width: digits.count * font.pixelSize * 0.7
        height: parent.height
        anchors.centerIn: parent
        Repeater {
            id: digits
            model: d.width
            WheelDigit {
                height: wheel.height
                font: root.font
                foreground: root.foreground
                order: d.order[index]
                digit: getDigit(index)
                minusVisible: root.value - Math.pow(10, order) >= root.lolim
                plusVisible: root.value + Math.pow(10, order) <= root.hilim
                onMinus: {
                    newValue = root.value - Math.pow(10, order)
                }
                onPlus: {
                    newValue = root.value + Math.pow(10, order)
                }
            }
        }
    }
}
