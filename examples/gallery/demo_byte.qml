import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0


Column {
    CaByte {
        width:300
        height: 50
        channel: 'calc'
        start: start.value
        end: end.value
        orientation: orientationGroup.orientation
    }
    OrientationGroup {
        id: orientationGroup
    }
    Row {
        Text {
            text: 'Start Bit'
        }
        Slider {
            id: start
            value: 0
            minimumValue: 0
            maximumValue: 31
            stepSize: 1
        }
    }
    Row {
        Text {
            text: 'End Bit'
        }
        Slider {
            id: end
            value: 15
            minimumValue: 0
            maximumValue: 31
            stepSize: 1
         }
    }
}
