import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSMessageButton
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.controls
    \brief Displays a push button

    The Message Button sets the CSData value when the button is pressed or released.
*/

CSControl {
    id: root
    /*!
      \qmlproperty string text
      This property holds the text on message button
    */
    property alias text: btn.text
    /*!
        This property holds the value to set to the CSData object when button is pressed.

        This value should be commensurate with the type of the process variable.
        (It is not wise to send a non-numeric string to a DOUBLE type data, for example.)
    */
    property string onMessage
    /*!
        This property holds the value to set to the CSData object when button is released

        This value should be commensurate with the type of the data.
        (It is not wise to send a non-numeric string to a DOUBLE type process variable, for example.)
    */
    property string offMessage

    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height - 4, true)

    implicitWidth: 100
    implicitHeight: 20

    StyledButton {
        id: btn
        backgroundColor: root.background
        foregroundColor: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
        font.pixelSize: root.font.size
        font.family: root.font.family
        anchors.fill: parent

        onPressedChanged: {
            if (pressed && onMessage)
                csdata.setValue(onMessage)
            else if(!pressed && offMessage)
                csdata.setValue(offMessage)
        }
    }
}
