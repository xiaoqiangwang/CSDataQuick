import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

GroupBox {
    property int fillMode: fillModeGroup.current.fillModeOption
    
    title: "Fill Mode"

    Column {
        ExclusiveGroup { id: fillModeGroup }
        RadioButton {
            property int fillModeOption: FillMode.FromEdge
            text: "From Edge"
            checked: true
            exclusiveGroup: fillModeGroup
        }
        RadioButton {
            property int fillModeOption: FillMode.FromCenter
            text: "From Center"
            exclusiveGroup: fillModeGroup
        }
    }
}

