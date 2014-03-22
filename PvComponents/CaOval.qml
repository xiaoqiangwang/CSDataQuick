import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    id: control

    Oval {
        id: oval
        anchors.fill: parent
        foreground: control.foreground
        lineWidth: control.lineWidth
        fillStyle: control.fill
        edgeStyle: control.edge
    }
}
