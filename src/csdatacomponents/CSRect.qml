import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CSRect
    \inqmlmodule CSDataQuick.Components
    \brief Display a rectangle

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
            fill: FillStyle.Outline
        }
    }
    \endqml

    \image rect.png
*/

CSGraphics {
    id: root
    implicitWidth: 100
    implicitHeight: 100
    PaintedRectangle {
        id: rect
        anchors.fill: parent
        lineWidth: root.lineWidth
        foreground: (colorMode == ColorMode.Alarm
                     || (dynamicAttribute.visibilityMode != VisibilityMode.Static
                         && !dynamicAttribute.connected))
                    ? root.alarmColor :root.foreground
        edgeStyle: root.edgeStyle
        fillStyle: root.fillStyle
    }
}
