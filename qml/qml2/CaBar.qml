import QtQuick 2.0
import QtQuick.Controls 1.0
import PvLibrary 1.0

ProgressBar {
    property alias channel: pv.channel
    property bool usePVLimits: true

    PvObject {
        id: pv
        channel: 'catest'
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
