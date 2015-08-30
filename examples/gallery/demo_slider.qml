import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0


Rectangle {
ColumnLayout {
    anchors.fill: parent
    CaSlider {
        width: 150
        height: 150
        channel: 'catest'
        direction: directionGroup.direction
        label: labelStyleGroup.labelStyle
    }
    DirectionGroup {
        id: directionGroup
    }
    LabelStyleGroup {
        id: labelStyleGroup
    }
}
}
