import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaMessageButton
    \inqmlmodule CSDataQuick.Components
    \brief Displays a push button

    The Message Button sets the process variable when the button is pressed or released.
*/

CaControl {
    id: root

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

    StyledButton {
        id: btn
        background: root.background
        foreground: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
        font.pixelSize: root.font.size
        font.family: root.font.family
        anchors.fill: parent

        onPressedChanged: {
            if (pressed)
                pv.setValue(onMessage)
            else
                pv.setValue(offMessage)
        }
    }
}
