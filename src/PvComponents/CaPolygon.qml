import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaPolygon
    \inqmlmodule PvComponents
    \brief Display a polygon shape
*/

CaGraphics {
    id: root
    /*!
        \qmltype list<Point> points

        Trace of the polygon
    */
    property alias points: polygon.points

    Polygon {
        id: polygon
        anchors.fill: parent
        foreground: root.foreground
        lineWidth: root.lineWidth
        fillStyle: root.fill
        edgeStyle: root.edge
    }
}
