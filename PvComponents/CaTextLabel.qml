import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

CaMonitor {
    id: label
    property alias align: label_control.horizontalAlignment
    property int format: TextFormat.Decimal
    property bool withUnits: false
    // limits
    property Limits limits: Limits {id: limits}

    Rectangle {
        id: panel
        color: label.background
        anchors.fill: parent
        Text {
            id: label_control
            font.pixelSize: label.fontSize
            font.family: label.fontFamily
            text: formatString(format, pv.value)
            color: label.foreground
            anchors.left: parent.left
        }
        Text {
            id: units
            font.pixelSize: label.fontSize
            font.family: label.fontFamily
            color: label.foreground
            anchors.left: label_control.right
            anchors.right: parent.right
            verticalAlignment: Text.AlignVCenter
            visible: withUnits
        }
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                units.text = pv.units;
                limits.prec = pv.prec
            }
        }
    }

    function formatString(format, value) {
        //if (pv.type == pv.Enum) {
        //    return pv.nostr[pv.value]
        //}

        if (value instanceof Array) {
            return value.toString()
        }
        var result = Utils.convert(format, value, limits.prec)
        return result
    }
}

