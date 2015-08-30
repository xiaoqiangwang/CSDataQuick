import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

/*!
    \qmltype CaMessageButton
    \inqmlmodule PvComponents
    \brief Displays a push button
*/

CaControl {
    id: messageButton

    /*!
      \qmlproperty string text
      Text on push button
    */
    property alias text: btn.text
    /*! Value to set the process variable when button is pressed */
    property var onMessage
    /*! Value to set the process variable when button is released */
    property var offMessage

    StyledButton {
        id: btn
        background: messageButton.background
        foreground: colorMode == ColorMode.Alarm ? messageButton.alarmColor : messageButton.foreground
        pixelSize: messageButton.fontSize
        fontFamily: messageButton.fontFamily
        anchors.fill: parent

        onPressedChanged: {
            if (pressed)
                pv.value = onMessage
            else
                pv.value = offMessage
        }
    }
}
