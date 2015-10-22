import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

GroupBox {
    property int direction: directionGroup.current.directionOption

    title: "Direction"

    Column {
        ExclusiveGroup { id: directionGroup }
        RadioButton {
            property int directionOption: Direction.Up
            text: "Up"
            checked: true
            exclusiveGroup: directionGroup
        }
        RadioButton {
            property int directionOption: Direction.Right
            text: "Right"
            exclusiveGroup: directionGroup
        }
        RadioButton {
            property int directionOption: Direction.Down
            text: "Down"
            exclusiveGroup: directionGroup
        }
        RadioButton {
            property int directionOption: Direction.Left
            text: "Left"
            exclusiveGroup: directionGroup
        }
    }
}

