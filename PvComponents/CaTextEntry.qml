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
    property Limits limits: Limits {id: limits}

    Rectangle {
        anchors.fill: parent
        BorderImage {
            source: 'images/button_down.png'
            anchors.leftMargin: -1
            anchors.topMargin: -2
            anchors.rightMargin: 1
            anchors.bottomMargin: 1
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
        anchors.bottomMargin: 2
        anchors.leftMargin: 4
        anchors.fill: parent

        onAccepted: {
            pv.value = text
        }
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                limits.prec = pv.prec
            }
        }
    }

    function formatString(format, value) {
        //if (pv.type == pv.Enum) {
        //    return pv.nostr[pv.value]
        //}
        var result = Utils.convert(format, value, limits.prec)
        return result
    }
}
