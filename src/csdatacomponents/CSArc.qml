import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CSArc
    \inqmlmodule CSDataQuick.Components
    \brief Display an arc curve

    The arc is drawn beginning at \l begin and extending \l span degrees
    counter clockwise. Negative \l span will effectively draw in clockwise.

    \qml
        CSArc {
            begin: 45
            span:  225
       }
    \endqml

    \image arc.png
*/

CSGraphics {
    id: root
    implicitWidth: 100
    implicitHeight: 100
    /*!
      \qmlproperty real begin
      The start of the arc in degrees.
    */
    property alias begin: arc.begin
    /*!
      \qmlproperty real span
      The length of the arc in degrees.
    */
    property alias span: arc.span

    Arc {
        id: arc
        anchors.fill: parent
        lineWidth: root.lineWidth
        fillStyle: root.fillStyle
        edgeStyle: root.edgeStyle
        foreground: (colorMode == ColorMode.Alarm
                     || (dynamicAttribute.visibilityMode != VisibilityMode.Static
                         && !dynamicAttribute.connected))
                    ? root.alarmColor :root.foreground
    }
}
