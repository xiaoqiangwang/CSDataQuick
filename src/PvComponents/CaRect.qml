import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaRect
    \inqmlmodule PvComponents
    \brief Display a rectangle
*/

CaGraphics {
    id: control
    PaintedRectangle {
        id: rect
        anchors.fill: parent
        lineWidth: control.lineWidth
        foreground: control.foreground
        edgeStyle: control.edge
        fillStyle: control.fill
    }
}
