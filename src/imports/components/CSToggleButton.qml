import QtQuick 2.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSToggleButton
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.controls
    \brief Displays a checkbox to toggle a boolean value.

    The Toggle Button sets the CSData value when the button is checked or cleared.
*/

CSControl {
    id: root
    /*!
      \qmlproperty string text
      This property holds a textual description of the button.
    */
    property alias text: btn.text
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

    StyledCheckBox {
        id: btn
        backgroundColor: root.background
        foregroundColor: (colorMode == ColorMode.Alarm && !Utils.inPuppet) ? root.alarmColor : root.foreground
        anchors.fill: parent
        fontSizeMode: Text.Fit
        font.family: UtilsJS.getBestFontSize(height-4).family
        Binding on font.pixelSize {
            when: fontSizeMode != Text.FixedSize
            value: height
        }
        enabled: csdata.accessRight & CSData.WriteAccess
        onCheckedChanged: {
            csdata.setValue(checked)
        }
    }

    Connections {
        target: csdata
        onValueChanged: {
            btn.checked = csdata.value
        }
    }
}
