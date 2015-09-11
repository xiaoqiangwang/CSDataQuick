import QtQuick 2.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaText
    \inqmlmodule PvComponents
    \brief Display a text label
*/

CaGraphics {
    id: text

    /*!
        \qmlproperty string text
        The text to display. It can be plain text or richt text.
    */
    property alias text: text_control.text
    /*!
        \qmlproperty enumeration align
        Sets the horizontal alignment of the text within the item width.

        The font used depends on the item height using function \l UtilsJS::getBestFontSize.
    */
    property alias align: text_control.horizontalAlignment

    Rectangle {
        color: background
        anchors.fill: parent
        Text {
            id: text_control
            color: foreground
            anchors.fill: parent
            font.pixelSize: text.fontSize
            font.family: text.fontFamily
        }
    }

    onHeightChanged: {
        var font = UtilsJS.getBestFontSize(height)
        fontSize = font.size
        fontFamily = font.family
    }
}
