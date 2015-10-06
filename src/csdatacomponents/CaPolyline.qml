import QtQuick 2.0

import CSData.Components 1.0

/*!
    \qmltype CaPolygon
    \inqmlmodule CSData.Components
    \brief Display a polyline

    Draws the polyline defined by the \l points array.
    Note that unlike CaPolygon, the last point is not connected to the first, unless the first point is repeated at the end.

    \qml
    CaPolyline {
        width: 100
        height: 100
        points: [
            Qt.point(10.0, 80.0),Qt.point(20.0, 10.0),
            Qt.point(80.0, 30.0),Qt.point(90.0, 70.0)
        ]
    }
    \endqml

    \image polyline.png

    \sa CaPolygon
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
        foreground: (colorMode == ColorMode.Alarm
                     || (dynamicAttr.visibilityMode != VisibilityMode.Static
                         && !dynamicAttr.connected))
                    ? root.alarmColor :root.foreground
        lineWidth: root.lineWidth
        edgeStyle: root.edge
    }
}
