import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaPolygon
    \inqmlmodule PvComponents
    \brief Display a polygon shape

    Draws the polygon defined by the \l points array. The first point is implicitly connected to the last point.

    \qml
    Row {
        spacing: 5
        CaPolygon {
            width: 100
            height: 100
            points: [
                Qt.point(10.0, 80.0), Qt.point(20.0, 10.0),
                Qt.point(80.0, 30.0),Qt.point(90.0, 70.0)
            ]
        }
        CaPolygon {
            width: 100
            height:100
            points: [
                Qt.point(10.0, 80.0),Qt.point(20.0, 10.0),
                Qt.point(80.0, 30.0),Qt.point(90.0, 70.0)
            ]
            fill: FillStyle.Outline
            edge: EdgeStyle.Dash
        }
    }
    \endqml

    \image polygon.png
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
        foreground: (colorMode == ColorMode.Alarm
                     || (dynamicAttr.visibilityMode != VisibilityMode.Static
                         && !dynamicAttr.connected))
                    ? root.alarmColor :root.foreground
        lineWidth: root.lineWidth
        fillStyle: root.fill
        edgeStyle: root.edge
    }
}
