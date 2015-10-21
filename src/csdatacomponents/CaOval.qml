import QtQuick 2.0

import CSDataQuick.Components 1.0


/*!
    \qmltype CaOval
    \inqmlmodule CSDataQuick.Components
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
    width: 100
    height: 100

    Oval {
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
