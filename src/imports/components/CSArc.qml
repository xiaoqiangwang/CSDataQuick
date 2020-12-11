import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CSArc
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.graphics
    \brief Display an arc curve.

    The arc is drawn beginning at \l begin and extending \l span degrees
    counter clockwise. Negative \l span will effectively draw in clockwise.
    An arrow can be drawn by either or both ends depending on \l arrowPosition.

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
    /*!
      \qmlproperty real begin
      This property holds the start of the arc in degrees.
    */
    property alias begin: arc.begin
    /*!
      \qmlproperty real span
      This property holds the length of the arc in degrees.
    */
    property alias span: arc.span
    /*!
      \qmlproperty int closure
      This property indicates how to close the gap.
    */
    property alias closure: arc.closure
    /*!
        \qmlproperty int arrowPosition

        This property indicates where the arrow is draw. \sa ArrowPosition,
    */
    property int arrowPosition: ArrowPosition.None

    Arc {
        id: arc
        anchors.fill: parent
        arrowPosition: root.arrowPosition
        lineWidth: root.lineWidth
        fillStyle: root.fillStyle
        edgeStyle: root.edgeStyle
        foreground: (root.colorMode == ColorMode.Alarm
                     || (root.dynamicAttribute.visibilityMode != VisibilityMode.Static
                         && !root.dynamicAttribute.connected)) && !Utils.inPuppet
                    ? root.alarmColor : root.foreground
    }
}
