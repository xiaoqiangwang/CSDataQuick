import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

CaMonitor {
    property bool usePVLimits: true
    property real value: 0.0
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property int  direction: 1 // up right down left

    Rectangle {
        id: panel
        color: background
        anchors.fill: parent
        border.width: 1
    }

    Rectangle {
        color: foreground
        width: direction == 0 || direction == 2 ? parent.width : parent.width * Math.min(1.0, (value - minimumValue) / (maximumValue - minimumValue))
        height: direction == 1 || direction == 3 ? parent.height : parent.height * Math.min(1.0, (value - minimumValue) / (maximumValue - minimumValue))
        anchors.left: direction == 1 ? parent.left : undefined
        anchors.right: direction == 3 ? parent.right : undefined
        anchors.top: direction == 2 ? parent.top : undefined
        anchors.bottom: direction == 0 ? parent.bottom : undefined
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected && usePVLimits) {
                if (pv.lodisplim < pv.updisplim) {
                    minimumValue = pv.lodisplim
                    maximumValue = pv.updisplim
                }
            }
        }
        onValueChanged: {
            value = pv.value
        }
    }
}
