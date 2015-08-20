import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0


Rectangle {
ColumnLayout {
    anchors.fill: parent
    CaMeter {
        width: 100
        height:150
        channel: 'catest'
        label: labelStyleGroup.labelStyle
    }
    LabelStyleGroup {
        id: labelStyleGroup
    }
}
}
