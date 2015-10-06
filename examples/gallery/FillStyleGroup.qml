import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

GroupBox {
    property int fillStyle: fillStyleGroup.current.fillStyleOption
    
    title: "Fill Style"

    Column {
        ExclusiveGroup { id: fillStyleGroup }
        RadioButton {
            property int fillStyleOption: FillStyle.Solid
            id: solid
            text: "Solid"
            checked: true
            exclusiveGroup: fillStyleGroup
        }
        RadioButton {
            property int fillStyleOption: FillStyle.Outline
            id: outline
            text: "Outline"
            exclusiveGroup: fillStyleGroup
        }
    }
}

