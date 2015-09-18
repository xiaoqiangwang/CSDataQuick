import QtQuick 2.0

import PvComponents 1.0


/*!
    \qmltype CaOval
    \inqmlmodule PvComponents
    \brief Display a circle or ellipse.

*/

CaGraphics {
    id: root

    Oval {
        anchors.fill: parent
        foreground: root.foreground
        lineWidth: root.lineWidth
        fillStyle: root.fill
        edgeStyle: root.edge
    }
}
