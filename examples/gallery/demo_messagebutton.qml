import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaTextUpdate {
            channel: 'calc'
            width: 100
        }
        CaMessageButton {
            text: 'reset'
            width: 80
            height: 20
            channel: 'calc'
            colorMode: colorModeGroup.colorMode
            onMessage: 0
            offMessage: 0
        }
        ColorModeGroup {
            id: colorModeGroup
        }
    }
}
