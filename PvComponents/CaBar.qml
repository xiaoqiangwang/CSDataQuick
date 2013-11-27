import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

CaMonitor {
    property bool usePVLimits: true
    property real value: 0.0
    property real minimum: 0.0
    property real maximum: 1.0

    Rectangle {
        id: panel
        color: background
        anchors.fill: parent
        border.width: 1
    }

    Rectangle {
        color: foreground
        height: parent.height
        anchors.left: parent.left
        width: parent.width * (value)
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
