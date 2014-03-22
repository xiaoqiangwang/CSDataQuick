import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0
import "utils.js" as JSUtils

CaControl {
    id: root
    // text format
    property alias align: textField.horizontalAlignment
    property int format: TextFormat.Decimal

    // limits
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
        color: root.foreground
        verticalAlignment: TextInput.AlignBottom
        activeFocusOnPress: false
        clip: true
        selectByMouse: true
        anchors.topMargin: 4
        anchors.bottomMargin: 2
        anchors.leftMargin: 2
        anchors.rightMargin: 2
        anchors.fill: parent
        onAccepted: {
            pv.value = text
        }
    }

    MouseArea {
        id: ma
        anchors.fill: textField
        acceptedButtons: Qt.LeftButton
        hoverEnabled: true
        propagateComposedEvents: true
        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false
        onEntered:{
            textField.focus = true
        }
        onExited: {
            textField.focus = false
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
