import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaTextUpdate {
            width: 100
            channel: 'catest'
            colorMode: colorModeGroup.colorMode
        }
        ColorModeGroup {
            id: colorModeGroup
        }
    }
}
