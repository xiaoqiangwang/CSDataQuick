import QtQuick 2.0

import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSText
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.graphics
    \brief Display a text label

    It can display both plain and rich text in HTML-style markup. The text may extend beyond the height and width specified for the Text item
    or may not use up all the specified height and width. The text typically does not fill all of the specified height for the Text item.

    The font used depends on the item height using function \l UtilsJS::getBestFontSize.

    \qml
    Column {
        spacing: 5
        CSText {
            width: 100
            height: 20
            text: 'Plain Text'
        }
        CSText {
            width: 100
            height: 20
            text: '<b>Rich</b> <i>Text</i>'
        }
    }
    \endqml

    \image text.png
*/

CSGraphics {
    id: root

    /*!
        \qmlproperty string text
        This property holds the text to display.

        It can be plain text or richt text defined using HTML markup.
    */
    property alias text: text_control.text
    /*!
        \qmlproperty enumeration align
        Sets the horizontal alignment of the text within the item width.

        The valid values for horizontalAlignment are Text.AlignLeft, Text.AlignRight, Text.AlignHCenter and Text.AlignJustify.
    */
    property alias align: text_control.horizontalAlignment

    /*!
        \qmlproperty font font
        The text font.
    */
    property alias font: text_control.font

    /*!
        \qmlproperty enumeration fontSizeMode

        \sa Text.fontSizeMode
    */
    property int fontSizeMode: Text.VerticalFit

    implicitWidth: text_control.implicitWidth
    implicitHeight: 20

    font.pixelSize: height

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
        fontSizeMode: root.fontSizeMode
        minimumPixelSize: 4
        verticalAlignment: Text.AlignHCenter
        onLinkActivated: Qt.openUrlExternally(link)
    }
}
