import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    Rectangle {
        id: rect
        border.width: lineWidth
        border.color: foreground
        color: fill == FillStyle.Solid ? foreground : "transparent"
        anchors.fill: parent
    }
}
