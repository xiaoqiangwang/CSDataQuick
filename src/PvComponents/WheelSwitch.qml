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
    property int activeDigit: -1

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
        property var step: []
        property real epsilon: Math.pow(10, -(decimal+3))
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

    /*! \internal
        format is specified in the form of %[width][.precision]f
    */
    function parseFormat() {
        if (d.format.charAt(0) != '%' || d.format.charAt(d.format.length - 1) != 'f')
            return
        var f = d.format.slice(1, -1).split('.')
        var width = Number(f[0])
        var decimal = Number(f[1])

        /* string pattern displayed if number does not fit the width */
        var undef = ''
        if (decimal > 0) {
            undef += '.'
            for (var i = 0; i < decimal; i++)
                undef += '*'
        }
        var tmp = undef.length
        for (var i = 0; i < width - tmp; i++)
            undef = '*' + undef

        /* step pattern */
        d.step = []
        if (decimal == 0) {
            for (var i=width -1 ; i>=0; i--)
                d.step.push(Math.pow(10, i))
        } else {
            for (var i = width - decimal - 2; i >= 0; i--)
                d.step.push(Math.pow(10, i))
            d.step.push(Number.NaN)
            for (var i = 0; i < decimal; i++)
                d.step.push(Math.pow(10, -(i+1)))
        }
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

    Frame {
        id: panel
        anchors.fill: parent
        color: root.background
        shadow: FrameShadow.Raise
    }

    MouseArea {
        id: mouseArea
        anchors.fill: root
        hoverEnabled: true

        onEntered: {
            if (activeDigit < 0 || activeDigit >= d.width)
                activeDigit = d.width - 1
            wheel.children[activeDigit].forceActiveFocus()
        }
        onExited: root.forceActiveFocus()
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
                digit: getDigit(index)
                minusVisible: !isNaN(digit) && (root.value - d.step[index]) >= root.lolim - d.epsilon
                plusVisible: !isNaN(digit) && (root.value +  d.step[index]) <= root.hilim + d.epsilon
                onMinus: {
                    newValue = (root.value -  d.step[index]).toFixed(d.decimal)
                }
                onPlus: {
                    newValue = (root.value +  d.step[index]).toFixed(d.decimal)
                }
                onActiveFocusChanged: {
                    if (activeFocus && activeDigit!=index)
                        activeDigit = index
                }
            }
        }
    }

    Keys.onLeftPressed: {
        var prevDigit = activeDigit
        while(--prevDigit >= 0) {
            if (wheel.children[prevDigit].minusVisible ||
                wheel.children[prevDigit].plusVisible) {
                wheel.children[prevDigit].forceActiveFocus()
                break
            }
        }
        if (prevDigit >= 0)
            activeDigit = prevDigit
    }
    Keys.onRightPressed: {
        var nextDigit = activeDigit
        while(++nextDigit < d.width) {
            if (wheel.children[nextDigit].minusVisible ||
                wheel.children[nextDigit].plusVisible) {
                wheel.children[nextDigit].forceActiveFocus()
                break
            }
        }
        if (nextDigit < d.width)
            activeDigit = nextDigit
    }
}
