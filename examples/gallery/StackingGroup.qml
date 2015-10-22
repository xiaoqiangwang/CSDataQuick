import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

GroupBox {
    property int stacking: stackingGroup.current.stackingOption

    title: "Stacking"

    Column {
        ExclusiveGroup { id: stackingGroup }
        RadioButton {
            property int stackingOption: Stacking.Column
            text: "Column"
            checked: true
            exclusiveGroup: stackingGroup
        }
        RadioButton {
            property int stackingOption: Stacking.Row
            text: "Row"
            exclusiveGroup: stackingGroup
        }
        RadioButton {
            property int stackingOption: Stacking.RowColumn
            text: "Row & Column"
            exclusiveGroup: stackingGroup
        }
    }
}

