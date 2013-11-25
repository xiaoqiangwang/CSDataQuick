import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

CaControl {
    property bool usePVLimits: true

    ProgressBar {
        id: bar_control
        minimumValue: 0
        maximumValue: 1

        Connections {
            target: pv
            onConnectionChanged: {
                if (pv.connected && usePVLimits) {
                    if (pv.lodisplim < pv.updisplim) {
                        bar_control.minimumValue = pv.lodisplim
                        bar_control.maximumValue = pv.updisplim
                    }
                }
            }
            onValueChanged: {
                bar_control.value = pv.value
            }
        }
    }
}
