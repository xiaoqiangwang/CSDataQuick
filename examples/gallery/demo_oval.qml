import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaOval {
            id: oval
            width: 200
            height: 200
            fill: fillStyleGroup.fillStyle
        }
        FillStyleGroup {
            id: fillStyleGroup
        }
    }
}
