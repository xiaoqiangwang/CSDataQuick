import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaPolygon
    \inqmlmodule PvComponents
    \brief Display a polyline
*/

CaGraphics {
    id: root
    /*!
        \qmltype list<Point> points

        Trace of the polyline
    */
    property alias points: polyline.points

    Polyline {
        id: polyline
        anchors.fill: parent
        foreground: root.foreground
        lineWidth: root.lineWidth
        edgeStyle: root.edge
    }
}
