import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    id: control
    property alias points: polyline.points

    Polygon {
        id: polyline
        foreground: control.foreground
        lineWidth: control.lineWidth
        fillStyle: control.fill
        edgeStyle: control.edge
        anchors.fill: parent
    }
}
