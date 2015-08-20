import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaChoiceButton {
            id: cb
            width: 100
            height: 20
            stacking: stackingGroup.stacking
            channel: 'bo'
        }
        StackingGroup {
            id: stackingGroup
        }
    }
}
