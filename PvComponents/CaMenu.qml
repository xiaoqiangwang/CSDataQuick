import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0


CaControl {
    ComboBox {
        id: combo
        property bool __first: true
        anchors.fill: parent
        onCurrentIndexChanged: {
            if (__first) __first = false
            else pv.setValue(currentIndex)
        }
    }
    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                combo.model = pv.strs
            } else {
                combo.model = []
            }
        }
        onValueChanged: {
            combo.currentIndex = pv.value
        }
    }
}
