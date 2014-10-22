import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaPolygon
    \inqmlmodule PvComponents
    \brief Display a polyline
*/

CaGraphics {
    id: control
    /*!
        \qmltype list<Point> points

        Trace of the polyline
    */
    property alias points: polyline.points

    Polyline {
        id: polyline
        anchors.fill: parent
        foreground: control.foreground
        lineWidth: control.lineWidth
        edgeStyle: control.edge
    }
}
