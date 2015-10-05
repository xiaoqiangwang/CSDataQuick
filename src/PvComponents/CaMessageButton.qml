import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaMessageButton
    \inqmlmodule PvComponents
    \brief Displays a push button

    The Message Button sets the process variable when the button is pressed or released.
*/

CaControl {
    id: messageButton

    /*!
      \qmlproperty string text
      Text on message button
    */
    property alias text: btn.text
    /*!
        Value to set the process variable when button is pressed.

        This value should be commensurate with the type of the process variable.
        (It is not wise to send a non-numeric string to a DOUBLE type process variable, for example.)
    */
    property var onMessage
    /*!
        Value to set the process variable when button is released

        This value should be commensurate with the type of the process variable.
        (It is not wise to send a non-numeric string to a DOUBLE type process variable, for example.)
    */
    property var offMessage

    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height - 4, true)

    fontFamily: font.family
    fontSize: font.size

    StyledButton {
        id: btn
        background: messageButton.background
        foreground: colorMode == ColorMode.Alarm ? messageButton.alarmColor : messageButton.foreground
        pixelSize: messageButton.fontSize
        fontFamily: messageButton.fontFamily
        anchors.fill: parent

        onPressedChanged: {
            if (pressed)
                pv.setValue(onMessage)
            else
                pv.setValue(offMessage)
        }
    }
}
