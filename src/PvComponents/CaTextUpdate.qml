import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaTextUpdate
    \inqmlmodule PvComponents
    \brief Display the value in given format.

    The value is format to string using \l format.

    The font used depends on the item height using function \l UtilsJS::getBestFontSize.
    If \l align is not Text.AlignLeft, it will attemp to find a font size so that the text can fit in item width.

    \qml
    Row {
        spacing: 5
        CaTextUpdate {
            width: 100
            height: 20
            source: 'catest.SCAN'
        }
        CaTextUpdate {
            width: 100
            height: 20
            source: 'catest'
        }
    }
    \endqml

    \image textupdate.png
*/

CaMonitor {
    id: root
    /*!
        \qmlproperty enumeration align
        Set the horizontal alignment of the text within the item width.
    */
    property alias align: label_control.horizontalAlignment
    /*!
        \qmlproperty enumeration format
        For all of the formats, the result depends on the number and the precision in \l limits.

        \list
        \li TextFormat.Decimal - Text with or without decimal point, e.g. 10.0 or 10.
        \li TextFormat.Exponential - Exponential notation, e.g. 1.00e+04.
        \li TextFormat.EngNotation - Engineering notation, e.g. 10.00e+03.
        \li TextFormat.Compact - Either in decimal or exponential form to be most compact.
        \li TextFormat.Truncated - Text is rounded to the largest integer, e.g 10000.
        \li TextFormat.Hexadecimal - The text is rounded to the nearest integer and shown in hexadecimal, e.g. 0x3e8.
        \li TextFormat.Octal - The text is rounded to the nearest integer and shown in octal, e.g. 01750.
        \li TextFormat.String - Same as decimal except that for large numbers or precision, it can be in exponential format.
        \endlist
    */
    property int format: TextFormat.Decimal
    /*! Display physical units if available */
    property bool showUnits: false
    /*! Operation limits and precision */
    property Limits limits: Limits {id: limits}

    RowLayout {
        anchors.fill: parent
        Text {
            id: label_control
            font.pixelSize: root.fontSize
            font.family: root.fontFamily
            //text: formatString(format, pv.value)
            color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
            clip: true
            Layout.fillWidth: true
        }
        Text {
            id: units
            font.pixelSize: root.fontSize
            font.family: root.fontFamily
            color: root.foreground
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            visible: showUnits && text != ''
        }
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                units.text = pv.units;
                limits.precChannel = pv.precision
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

    onFormatChanged: {
        label_control.text = formatString(format, pv.value)
    }

    onHeightChanged: {
        var font = UtilsJS.getBestFontSize(height)
        fontSize = font.size
        fontFamily = font.family
    }

    /*! \internal */
    function formatString(format, value) {
        if (pv.fieldType == CSData.Enum)
            return pv.stateStrings[value]
        if (pv.fieldTYpe == CSData.String)
            return value
        if (pv.fieldType == CSData.Char && value instanceof Array)
            return arrayToString(value)
        var result = Utils.convert(format, value, limits.prec)
        return result
    }

    /*! \internal */
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

