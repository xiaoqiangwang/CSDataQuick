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
        anchors.rightMargin: control.lineWidth == 0 ? 1 : 0
        anchors.bottomMargin: control.lineWidth == 0 ? 1 : 0
        lineWidth: control.lineWidth
        foreground: control.foreground
        edgeStyle: control.edge
        fillStyle: control.fill
    }
}
