import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaPolygon
    \inqmlmodule PvComponents
    \brief Display a polygon shape
*/

CaGraphics {
    id: control
    /*!
        \qmltype list<Point> points

        Trace of the polygon
    */
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
