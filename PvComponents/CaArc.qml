import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    id: control
    property alias begin: arc.begin
    property alias span: arc.span

    Arc {
        id: arc
        anchors.fill: parent
        lineWidth: control.lineWidth
        fillStyle: control.fill
        edgeStyle: control.edge
        foreground: control.foreground
    }
}
