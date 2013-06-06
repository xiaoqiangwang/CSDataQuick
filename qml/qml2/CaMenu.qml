import QtQuick 2.0
import QtQuick.Controls 1.0
import PvLibrary 1.0

ComboBox {
    property alias channel: pv.channel

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
        pv.value = currentIndex
    }
}
