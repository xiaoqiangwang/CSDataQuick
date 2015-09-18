import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaRect
    \inqmlmodule PvComponents
    \brief Display a rectangle
*/

CaGraphics {
    id: root
    PaintedRectangle {
        id: rect
        anchors.fill: parent
        lineWidth: root.lineWidth
        foreground: root.foreground
        edgeStyle: root.edge
        fillStyle: root.fill
    }
}
