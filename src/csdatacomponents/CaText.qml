import QtQuick 2.0

import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaText
    \inqmlmodule CSDataQuick.Components
    \brief Display a text label

    It can display both plain and rich text. The text may extend beyond the height and width specified for the Text item
    or may not use up all the specified height and width. The text typically does not fill all of the specified height for the Text item.

    The font used depends on the item height using function \l UtilsJS::getBestFontSize.

    \qml
    Column {
        spacing: 5
        CaText {
            width: 100
            height: 20
            text: 'Plain Text'
        }
        CaText {
            width: 100
            height: 20
            text: '<b>Rich</b> <i>Text</i>'
        }
    }
    \endqml

    \image text.png
*/

CaGraphics {
    id: root

    /*!
        \qmlproperty string text
        The text to display. It can be plain text or richt text defined using HTML markup.
    */
    property alias text: text_control.text
    /*!
        \qmlproperty enumeration align
        Sets the horizontal alignment of the text within the item width.
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
    }

    Rectangle {
        z: 1
        color: ColorMap.invalid_alarm
        anchors.fill: parent
        visible: (colorMode == ColorMode.Alarm || dynamicAttribute.visibilityMode != VisibilityMode.Static) && !dynamicAttribute.connected
    }

    Text {
        id: text_control
        color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
        anchors.fill: parent
    }

    onHeightChanged: {
        var font = UtilsJS.getBestFontSize(height, false)
        root.font.pixelSize = font.size
        root.font.family = font.family
    }
}
