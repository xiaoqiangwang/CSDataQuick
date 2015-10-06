import QtQuick 2.0

import CSData.Components 1.0


/*!
    \qmltype CaOval
    \inqmlmodule CSData.Components
    \brief Display a circle or ellipse.

    The ellipse is drawn always within the item boundary.

    \qml
    Row {
        spacing: 5
        CaOval {
            width: 50
            height: 50
        }
        CaOval {
            width: 100
            height: 50
            fillStyle: FillStyle.Outline
        }
    }
    \endqml

    \image oval.png
*/

CaGraphics {
    id: root

    Oval {
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
