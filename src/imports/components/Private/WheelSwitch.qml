import QtQml 2.0
import QtQuick 2.0

import CSDataQuick.Components 1.0

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
        property string defaultFormat: calculateFormat(root.hilim, root.lolim, root.prec)
        property string numberFormat: parseFormat(root.format)
        property int width: 1
        property int decimal: 0
        property string prefix: ''
        property string valueString: convert(root.value)
        property string postfix: ''
        property var step: []
        property real epsilon: Math.pow(10, -(decimal+3))
        property int divisor: Math.pow(10, decimal)
        onNumberFormatChanged: computeFormat()
    }

    /*!
        \internal
        default format based on limit and precision
    */
    function calculateFormat(high, low, precision) {
        var max = Math.max(Math.abs(high), Math.abs(low))
        var width
        if (max >= 1.0) {
            width = Math.floor((Math.log(max)/Math.LN10)) + 3 + precision
        } else
            width = 3 + precision
        var format = '% %1.%2f'.arg(width).arg(precision)
        return format
    }

    /*! \internal
        parser the format specified in the form of [prefix]%[+ ][width][.precision]f[postfix]
        and return the number format
    */
    function parseFormat(full_format) {
        if (full_format.length == 0)
            return d.defaultFormat

        // prefix text
        var prefix_size = full_format.indexOf('%');
        if (prefix_size == -1) {
            console.error('Invalid format with no "%"')
            return d.defaultFormat
        }
        d.prefix = full_format.substring(0, prefix_size)

        // postfix text
        var postfix_index = full_format.indexOf('f', prefix_size)
        if (postfix_index == -1) {
            console.error('Invalid format with no "f" formatter')
            return d.defaultFormat
        }
        postfix_index += 1
        d.postfix = full_format.substring(postfix_index)

        // get the number and validate
        var format = full_format.substring(prefix_size, postfix_index)
        var groups = format.match(/%(\+|\ )?(\d+)?(\.(\d+))?f/i)
        if (!groups)
            return d.defaultFormat
        return format
    }

    /*! \internal
        compute number of digits and decimals from number format
    */
    function computeFormat() {
        var groups = d.numberFormat.match(/%(\+|\ )?(\d+)?(\.(\d+))?f/i)
        var width = Number(groups[2])
        var decimal = groups[4] ? Number(groups[4]) : 0

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
        var res =  Utils.format(d.numberFormat, number);
        if (res.length > d.width) {
            res = d.undef
        }
        return res
    }

    function getDigit(index) {
        /* during format change, index can overflow */
        if (index >= d.valueString.length)
            return ' '
        else
            return d.valueString[index]
    }

    StyledFrame {
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
            if (root.activeDigit < 0 || root.activeDigit >= d.width)
                root.activeDigit = d.width - 1
            wheel.children[root.activeDigit].forceActiveFocus()
        }
        onExited: root.forceActiveFocus()
    }

    Row {
        visible: !input.visible
        width: (prefix.count + digits.count + postfix.count)* root.font.pixelSize * 0.7
        height: parent.height
        anchors.centerIn: parent

        Repeater {
            id: prefix
            model: d.prefix.length
            Text {
                height: parent.height
                font: root.font
                color: root.foreground
                text: d.prefix.charAt(index)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        Row {
            id: wheel
            height: parent.height

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
                        root.newValue = Math.round((root.value - d.step[index]) * d.divisor) / d.divisor
                    }
                    onPlus: {
                        root.newValue = Math.round((root.value + d.step[index]) * d.divisor) / d.divisor
                    }
                    onActiveFocusChanged: {
                        if (activeFocus && root.activeDigit!=index)
                            root.activeDigit = index
                    }
                }
            }
        }
        Repeater {
            id: postfix
            model: d.postfix.length
            Text {
                height: parent.height
                font: root.font
                color: root.foreground
                text: d.postfix.charAt(index)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Text {
        id: input
        anchors.centerIn: parent
        text: ''
        visible: false
    }

    Keys.onLeftPressed: {
        var prevDigit = activeDigit
        while(--prevDigit >= 0) {
            if (wheel.children[prevDigit].minusVisible ||
                wheel.children[prevDigit].plusVisible) {
                wheel.children[prevDigit].forceActiveFocus()
                if (prevDigit >= 0)
                    activeDigit = prevDigit
                break
            }
        }
    }
    Keys.onRightPressed: {
        var nextDigit = activeDigit
        while(++nextDigit < d.width) {
            if (wheel.children[nextDigit].minusVisible ||
                wheel.children[nextDigit].plusVisible) {
                wheel.children[nextDigit].forceActiveFocus()
                if (nextDigit < d.width)
                    activeDigit = nextDigit
                break
            }
        }
    }
    Keys.onPressed: {
        switch(event.key) {
        case Qt.Key_0:
        case Qt.Key_1:
        case Qt.Key_2:
        case Qt.Key_3:
        case Qt.Key_4:
        case Qt.Key_5:
        case Qt.Key_6:
        case Qt.Key_7:
        case Qt.Key_8:
        case Qt.Key_9:
        case Qt.Key_Period:
            input.visible = true
            input.text += event.text
            break
        case Qt.Key_Minus:
            input.visible = true
            if (input.text.charAt(0) != '-')
                input.text = '-' + input.text
            break
        case Qt.Key_Plus:
            input.visible = true
            if (input.text.charAt(0) == '-')
                input.text = input.text.substring(1, input.text.length)
            break
        case Qt.Key_Delete:
        case Qt.Key_Backspace:
            input.text = input.text.substring(0, input.text.length - 1)
            break
        case Qt.Key_Escape:
            input.text = ''
            input.visible = false
            break
        case Qt.Key_Enter:
        case Qt.Key_Return:
            var number = parseFloat(input.text)
            if (number >= root.lolim && number <= root.hilim)
                newValue = number
            input.text = ''
            input.visible = false
            break
        }
    }
}
