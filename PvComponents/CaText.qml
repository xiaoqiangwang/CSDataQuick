import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    id: text
    property alias text: text_control.text
    property alias align: text_control.horizontalAlignment

    Rectangle {
        color: background
        anchors.fill: parent
        Text {
            id: text_control
            color: foreground
            anchors.fill: parent
            font.pixelSize: text.fontSize
            font.family: text.fontFamily
        }
    }
}
