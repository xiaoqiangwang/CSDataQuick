import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaTextEntry
    \inqmlmodule PvComponents
    \brief Display an editable text
*/

CaControl {
    id: root
    /*!
        \qmlproperty enumeration align
        Sets the horizontal alignment of the text within the item width.

        The font used depends on the item height using function \l UtilsJS::getBestFontSize.
    */
    property alias align: textField.horizontalAlignment
    /*!
        \qmlproperty TextFormat format
    */
    property int format: TextFormat.Decimal

    /*! The low high operation range and precision */
    property Limits limits: Limits {id: limits}

    Rectangle {
        anchors.fill: parent
        BorderImage {
            source: 'images/button_down.png'
            anchors.leftMargin: 3
            border {left: 3; right: 3; top: 3; bottom: 3;}
            horizontalTileMode: BorderImage.Stretch
            verticalTileMode: BorderImage.Stretch
            width: parent.width
            height: parent.height
        }
        color: root.background
    }

    TextInput {
        id: textField
        font.pixelSize: root.fontSize
        font.family: root.fontFamily
        color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
        verticalAlignment: TextInput.AlignBottom
        activeFocusOnPress: true
        readOnly: !pv.writable
        clip: true
        selectByMouse: true
        anchors.topMargin: 4
        anchors.bottomMargin: 2
        anchors.leftMargin: 2
        anchors.rightMargin: 2
        anchors.fill: parent
        onAccepted: {
            if (!pv.writable)
                return
            var value;
            switch (pv.type) {
            case PvObject.String:
                pv.value = text
                break
            case PvObject.Enum:
                var found = false
                for(var i=0; i<pv.strs.length; i++) {
                    if (text == pv.strs[i]) {
                        found = true
                        break
                    }
                }
                if (found)
                    pv.value = text
                else {
                    value = Utils.parse(format, text)
                    if (!isNaN(value))
                        pv.value = value
                }
                break
            case PvObject.Integer:
            case PvObject.Long:
                value = Utils.parse(format, text)
                if (!isNaN(value))
                    pv.value = value
                break
            case PvObject.Char:
                pv.value = text
                break
            default:
                value = Utils.parse(format, text)
                if (!isNaN(value))
                    pv.value = value
            }
        }
    }

    MouseArea {
        anchors.fill: textField
        acceptedButtons: Qt.NoButton
        hoverEnabled: true
        onEntered: textField.focus = true
        onExited: {
            textField.focus = false
            textField.text = formatString(format, pv.value)
        }
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                limits.precChannel = pv.prec
            }
        }
        onValueChanged: {
            textField.text = formatString(format, pv.value)
            if (!textField.activeFocus)
                textField.cursorPosition = 0
        }
    }

    onFormatChanged: {
        textField.text = formatString(format, pv.value)
    }

    onHeightChanged: {
        var font = UtilsJS.getBestFontSize(height, true)
        fontSize = font.size
        fontFamily = font.family
    }

    /*!
        \internal
        Format the value based on PV type.
    */
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

    /*!
      \internal
      Format char array to string.
    */
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