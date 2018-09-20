import QtQuick 2.0

import CSDataQuick.Components 1.0


/*!
    \qmltype CSOval
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.graphics
    \brief Display a circle or ellipse.

    The ellipse is drawn always within the item boundary.

    \qml
    Row {
        spacing: 5
        CSOval {
            width: 50
            height: 50
        }
        CSOval {
            width: 100
            height: 50
            fillStyle: FillStyle.Outline
        }
    }
    \endqml

    \image oval.png
*/

CSGraphics {
    id: root

    Oval {
        anchors.fill: parent
        foreground: (colorMode == ColorMode.Alarm
                     || (dynamicAttribute.visibilityMode != VisibilityMode.Static
                         && !dynamicAttribute.connected)) && !Utils.inPuppet
                    ? root.alarmColor : root.foreground
        lineWidth: root.lineWidth
        fillStyle: root.fillStyle
        edgeStyle: root.edgeStyle
    }
}
