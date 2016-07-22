import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0


Rectangle {
ColumnLayout {
    anchors.fill: parent
    CSIndicator {
        width: 100
        height: 50
        source: 'catest'
        colorMode: colorModeGroup.colorMode
        direction: directionGroup.direction
        labelStyle: labelStyleGroup.labelStyle
    }
    ColorModeGroup {
        id: colorModeGroup
    }
    DirectionGroup {
        id: directionGroup
    }
    LabelStyleGroup {
        id: labelStyleGroup
    }
}
}
