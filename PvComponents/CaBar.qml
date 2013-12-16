import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

CaMonitor {
    id: control

    property int label: LabelStyle.Frame
    property alias direction: bar.direction
    property alias fillMode: bar.fillMode
    property Limits limits : Limits{}

    StyledBar {
        id: bar
        anchors.fill: parent
        foreground: control.foreground
        background: control.background
        minimumValue: limits.lopr
        maximumValue: limits.hopr
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                if (pv.lodisplim < pv.updisplim) {
                    limits.loprChannel = pv.lodisplim
                    limits.hoprChannel = pv.updisplim
                }
            }
        }
        onValueChanged: {
            bar.value = pv.value
        }
    }
}
