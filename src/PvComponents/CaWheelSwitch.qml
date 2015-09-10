import QtQuick 2.0

import PvComponents 1.0
import "utils.js" as UtilsJS

CaControl {
    id: root

    property alias format: wheel.format
    property Limits limits: Limits {}

    readonly property var font: UtilsJS.getBestFontSize(height, 1)

    WheelSwitch {
        id: wheel
        anchors.fill: parent
        lolim: limits.lopr
        hilim: limits.hopr
        prec: limits.prec
        value: pv.value
        foreground: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
        background: root.background
        onNewValueChanged: pv.value = newValue
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                if (pv.lodisplim < pv.updisplim) {
                    limits.loprChannel = pv.lodisplim
                    limits.hoprChannel = pv.updisplim
                }
                limits.precChannel = pv.prec
            }
        }
    }
}
