import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

GroupBox {
    property int orientation: orientationGroup.current.orientationOption

    title: "Orientation"

    Column {
        ExclusiveGroup { id: orientationGroup }
        RadioButton {
            property int orientationOption: Qt.Horizontal
            text: "Horizontal"
            checked: true
            exclusiveGroup: orientationGroup
        }
        RadioButton {
            property int orientationOption: Qt.Vertical
            text: "Vertical"
            exclusiveGroup: orientationGroup
        }
    }
}

