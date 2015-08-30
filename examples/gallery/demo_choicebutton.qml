import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaChoiceButton {
            channel: 'bo'
            width: 100
            height: 40
            stacking: stackingGroup.stacking
        }
        StackingGroup {
            id: stackingGroup
        }
    }
}
