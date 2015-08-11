import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

GroupBox {
    property int edgeStyle: edgeStyleGroup.current.edgeStyleOption

    title: "Edge Style"

    Column {
        ExclusiveGroup { id: edgeStyleGroup }
        RadioButton {
            property int edgeStyleOption: EdgeStyle.Solid
            id: solidEdge
            text: "Solid"
            checked: true
            exclusiveGroup: edgeStyleGroup
        }
        RadioButton {
            property int edgeStyleOption: EdgeStyle.Dash
            id: dashEdge
            text: "Dash"
            exclusiveGroup: edgeStyleGroup
        }
    }
}

