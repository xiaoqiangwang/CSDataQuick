import QtQuick 2.0

import PvComponents 1.0

import "utils.js" as UtilsJS
Item {
    id: wheel
    width: 150
    height: 50
    property real value: -11.23
    property string format: '%06.2f'
    property real hilim: 20
    property real lolim: -20
    property int prec: 0

    function parse(format) {
        if (format.charAt(0) != '%' || format.charAt(format.length - 1) != 'f')
            return
        var f = format.slice(2, -1).split('.')
        return [Number(f[0]), Number(f[0]) - Number(f[1]), Number(f[1])]
    }

    function convert(number) {
        var res =  Utils.format(format, value);
        return res
    }

    function getNumDigit() {

    }

    onValueChanged: {
        var res = convert(value).split('.')
    }

    Row {
        anchors.fill: parent
        Text {
            id: sign
            y: parent.height / 4
            height: parent.height / 2
            font.pixelSize: UtilsJS.getBestFontSize(height)
            width: parent.width / parse(format)[0]
            verticalAlignment: Text.AlignBottom
            text: value > 0 ? '' : '-'
        }

        Repeater {
            id: integer
            model: parse(format)[1]
            WheelDigit {
                width: parent.width / parse(format)[0]
                height: parent.height
                order: parse(format)[1] - index
            }
        }
        Text {
            id: point
            y: parent.height / 4
            height: parent.height / 2
            font.pixelSize: UtilsJS.getBestFontSize(height)
            width: parent.width / parse(format)[0]
            verticalAlignment: Text.AlignBottom
            text: '.'
        }
        Repeater {
            id: fraction
            model: parse(format)[2]
            WheelDigit {
                width: parent.width / parse(format)[0]
                height: parent.height
                order: -1 * (index + 1)
            }
        }
    }
}
