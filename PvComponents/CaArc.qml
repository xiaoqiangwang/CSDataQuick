import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaArc
    \inqmlmodule PvComponents
    \brief Display an arc curve

    The arc is drawn beginning at \l begin and extending \l span degrees
    counter clockwise.

    \qml
        CaArc {
            begin: 45
            span:  225
       }
    \endqml

    \image arc.png
*/

CaGraphics {
    id: control
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
        lineWidth: control.lineWidth
        fillStyle: control.fill
        edgeStyle: control.edge
        foreground: control.foreground
    }
}
