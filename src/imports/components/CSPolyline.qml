import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CSPolygon
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.graphics
    \brief Display a polyline

    Draws the polyline defined by the \l points array.
    Note that unlike CSPolygon, the last point is not connected to the first, unless the first point is repeated at the end.

    \qml
    CSPolyline {
        width: 100
        height: 100
        points: [
            Qt.point(10.0, 80.0),Qt.point(20.0, 10.0),
            Qt.point(80.0, 30.0),Qt.point(90.0, 70.0)
        ]
    }
    \endqml

    \image polyline.png

    \sa CSPolygon
*/

CSGraphics {
    id: root
    implicitWidth: polyline.implicitWidth
    implicitHeight: polyline.implicitHeight
    /*!
        \qmlproperty list<point> points

        This property holds the trace of the polyline as a list of points.
    */
    property alias points: polyline.points
    /*!
        \qmlproperty int arrowPosition

        This property indicates where the arrow is draw. \sa ArrowPosition,
    */
    property int arrowPosition: ArrowPosition.None

    /*! \internal */
    QtObject {
        id: d
        property double oldWidth: NaN
        property double oldHeight: NaN
    }

    onWidthChanged: {
        if (Utils.inPuppet)
            return

        var newpoints = []
        if (d.oldWidth !== NaN) {
            for (var i=0; i<points.length; i++)
                newpoints.push(Qt.point(points[i].x * width / d.oldWidth, points[i].y))
            points = newpoints
        }
        d.oldWidth = width
    }

    onHeightChanged: {
        if (Utils.inPuppet)
            return

        var newpoints = []
        if (d.oldHeight !== NaN) {
            for (var i=0; i<points.length; i++)
                newpoints.push(Qt.point(points[i].x, points[i].y * height / d.oldHeight))
            points = newpoints
        }
        d.oldHeight = height
    }

    Polyline {
        id: polyline
        anchors.fill: parent
        foreground: (colorMode == ColorMode.Alarm
                     || (dynamicAttribute.visibilityMode != VisibilityMode.Static
                         && !dynamicAttribute.connected)) && !Utils.inPuppet
                    ? root.alarmColor : root.foreground
        lineWidth: root.lineWidth
        edgeStyle: root.edgeStyle
        arrowPosition: root.arrowPosition
    }
}
