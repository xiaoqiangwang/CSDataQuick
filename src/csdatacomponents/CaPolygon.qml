import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CaPolygon
    \inqmlmodule CSDataQuick.Components
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
    implicitWidth: 100
    implicitHeight: 100
    /*!
        \qmltype list<Point> points

        Trace of the polygon
    */
    property alias points: polygon.points

    Polygon {
        id: polygon
        anchors.fill: parent
        foreground: (colorMode == ColorMode.Alarm
                     || (dynamicAttribute.visibilityMode != VisibilityMode.Static
                         && !dynamicAttribute.connected))
                    ? root.alarmColor :root.foreground
        lineWidth: root.lineWidth
        fillStyle: root.fillStyle
        edgeStyle: root.edgeStyle
    }
}
