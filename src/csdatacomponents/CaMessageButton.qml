import QtQuick 2.0

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
    implicitWidth: 100
    implicitHeight: 20
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
    property string onMessage
    /*!
        Value to set the process variable when button is released

        This value should be commensurate with the type of the process variable.
        (It is not wise to send a non-numeric string to a DOUBLE type process variable, for example.)
    */
    property string offMessage

    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height - 4, true)

    StyledButton2 {
        id: btn
        backgroundColor: root.background
        foregroundColor: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
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
