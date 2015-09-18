import QtQuick 2.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaText
    \inqmlmodule PvComponents
    \brief Display a text label
*/

CaGraphics {
    id: root

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
    /*!
        \qmlproperty font font
        The font used to display the text.
    */
    property alias font: text_control.font

    Rectangle {
        color: background
        anchors.fill: parent
        Text {
            id: text_control
            color: foreground
            anchors.fill: parent
        }
    }

    onHeightChanged: {
        var font = UtilsJS.getBestFontSize(height)
        font.pixelSize = font.size
        font.family = font.family
    }
}
