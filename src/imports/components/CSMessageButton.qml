import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSMessageButton
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.controls
    \brief Displays a push button.

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
    property string pressMessage
    /*!
        \obsolete

        Use pressMessage instead.
    */
    property alias onMessage: root.pressMessage
    /*!
        This property holds the value to set to the CSData object when button is released

        This value should be commensurate with the type of the data.
        (It is not wise to send a non-numeric string to a DOUBLE type process variable, for example.)
    */
    property string releaseMessage
    /*!
        \obsolete

        Use releaseMessage instead.
    */
    property alias offMessage: root.releaseMessage
    /*!
        \qmlproperty font font
        The text font.
    */
    property alias font: btn.font
    
    /*!
        \qmlproperty enumeration fontSizeMode
        This property specifies how the font size of the displayed text is determined.

        \sa {Text::} {fontSizeMode}
    */
    property alias fontSizeMode: btn.fontSizeMode

    implicitWidth: btn.implicitWidth
    implicitHeight: btn.implicitHeight

    StyledButton {
        id: btn
        anchors.fill: parent
        backgroundColor: root.background
        foregroundColor: (root.colorMode == ColorMode.Alarm && !Utils.inPuppet) ? root.alarmColor : root.foreground
        fontSizeMode: Text.Fit
        font.family: UtilsJS.getBestFontSize(height - 4, true).family
        onPressedChanged: {
            if (pressed && root.pressMessage)
                csdata.setValue(root.pressMessage)
            else if(!pressed && root.releaseMessage)
                csdata.setValue(root.releaseMessage)
        }
    }
}
