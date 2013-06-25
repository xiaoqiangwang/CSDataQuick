import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0


ComboBox {
    property alias channel: pv.channel
    property bool __first: true
    PvObject {
        id: pv
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                model = pv.strs
            } else {
                model = []
            }
        }
        onValueChanged: {
            currentIndex = pv.value
        }
    }
    onCurrentIndexChanged: {
        if (__first) __first = false
        else pv.setValue(currentIndex)
    }
}
