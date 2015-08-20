import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0


Rectangle {
ColumnLayout {
    anchors.fill: parent
    CaBar {
        width: 100
        height: 50
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
