import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CSRect
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.graphics
    \brief Display a rectangle.

    The rectangle is drawn always within the item boundary.

    \qml
    Row {
        spacing: 5
        CSRect {
            width: 100
            height: 100
        }
        CSRect {
            width: 100
            height: 100
            lineWidth: 5
            fillStyle: FillStyle.Outline
        }
        CSRect {
            width: 150
            height: 30
            radiusX: 10
            radiusY: 10
            fillStyle: FillStyle.Outline
        }
    }
    \endqml

    \image rect.png
*/

CSGraphics {
    id: root
    /*! the x radius of the ellipses defining the corners of the rounded rectangle */
    property real radiusX: 0
    /*! the y radius of the ellipses defining the corners of the rounded rectangle */
    property real radiusY: 0

    PaintedRectangle {
        id: rect
        anchors.fill: parent
        radiusX: root.radiusX
        radiusY: root.radiusY
        lineWidth: root.lineWidth
        foreground: (root.colorMode == ColorMode.Alarm
                     || (root.dynamicAttribute.visibilityMode != VisibilityMode.Static
                         && !root.dynamicAttribute.connected)) && !Utils.inPuppet
                    ? root.alarmColor : root.foreground
        edgeStyle: root.edgeStyle
        fillStyle: root.fillStyle
    }
}
