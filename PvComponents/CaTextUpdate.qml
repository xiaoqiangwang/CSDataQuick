import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

CaMonitor {
    id: label
    property alias align: label_control.horizontalAlignment
    property int format: TextFormat.Decimal
    property bool showUnits: false
    // limits
    property Limits limits: Limits {id: limits}

    Rectangle {
        id: panel
        color: label.background
        anchors.fill: parent
        RowLayout {
            anchors.fill: parent
            Text {
                id: label_control
                font.pixelSize: label.fontSize
                font.family: label.fontFamily
                //text: formatString(format, pv.value)
                color: colorMode == ColorMode.Alarm ? label.alarmColor : label.foreground
                clip: true
                Layout.fillWidth: true
            }
            Text {
                id: units
                font.pixelSize: label.fontSize
                font.family: label.fontFamily
                color: label.foreground
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                visible: showUnits && text != ''
            }
        }
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                units.text = pv.units;
                limits.precChannel = pv.prec
            }
        }
        onValueChanged: {
            label_control.text = formatString(format, pv.value)
            // MEDM Compat: automatic adjust font size only if it is not left aligned
            if (align == Text.AlignLeft)
                return
            while(fontSize > 8 && label_control.paintedWidth > label_control.width) {
                fontSize -= 1
            }
        }
    }

    onHeightChanged: {
        var font = UtilsJS.getBestFontSize(height)
        fontSize = font.size
        fontFamily = font.family
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

    function arrayToString(array) {
        var s = ''
        for(var i = 0; i < array.length; i++) {
            var v = array[i]
            if (v == 0)
                break
            s += String.fromCharCode(v)
        }
        return s
    }
}

