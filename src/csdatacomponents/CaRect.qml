import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CaRect
    \inqmlmodule CSDataQuick.Components
    \brief Display a rectangle

    The rectangle is drawn always within the item boundary.

    \qml
    Row {
        spacing: 5
        CaRect {
            width: 100
            height: 100
        }
        CaRect {
            width: 100
            height: 100
            lineWidth: 5
            fill: FillStyle.Outline
        }
    }
    \endqml

    \image rect.png
*/

CaGraphics {
    id: root
    width: 100
    height: 100
    PaintedRectangle {
        id: rect
        anchors.fill: parent
        lineWidth: root.lineWidth
        foreground: (colorMode == ColorMode.Alarm
                     || (dynamicAttribute.visibilityMode != VisibilityMode.Static
                         && !dynamicAttribute.connected))
                    ? root.alarmColor :root.foreground
        edgeStyle: root.edge
        fillStyle: root.fill
    }
}
