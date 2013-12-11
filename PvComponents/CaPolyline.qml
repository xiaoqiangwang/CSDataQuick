import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    id: control
    property alias points: polyline.points

    Polyline {
        id: polyline
        anchors.fill: parent
        foreground: control.foreground
        lineWidth: control.lineWidth
        edgeStyle: control.edge
    }
}
