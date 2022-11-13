import QtQuick 2.1
import QtQuick.Layouts 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Compat 1.0 as Compat
import "utils.js" as UtilsJS


/*!
    \qmltype CSChoiceButton
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.controls
    \brief Display data's enumerate.

    The Choice Button is used for ENUM data type and is a means for picking
    the ENUM choices via buttons.

    \qml
    CSChoiceButton {
        source: 'bo'
        stacking: Stacking.Row
    }
    \endqml

    \image choicebutton.png
*/

CSControl {
    id: root
    /*!
      This property holds the layout for the choice buttons. Default is Stacking.Column.

      \list
        \li Stacking.Column: The buttons are arranged in a column
        \li Stacking.Row: The buttons are arranged in a row
        \li Stacking.RowColumn: The buttons are automatically arranged in rows and columns
      \endlist
    */
    property int stacking: Stacking.Column
    /*!
        \qmlproperty font font
        The text font.
    */
    property alias font: hiddenText.font
    
    /*!
        \qmlproperty enumeration fontSizeMode
        This property specifies how the font size of the displayed text is determined.

        \sa {Text::} {fontSizeMode}
    */
    property int fontSizeMode: Text.Fit
    
    font.family: UtilsJS.getBestFontSize(stacking==Stacking.Column ? height / csdata.stateStrings.length - 4 : height - 4, true).family

    implicitWidth: loader.implicitWidth 
    implicitHeight: loader.implicitHeight

    Compat.ExclusiveButtonGroup { id: radioInputGroup }

    Loader {
        id: loader
        anchors.fill: parent
        enabled: csdata.accessRight & CSData.WriteAccess
        sourceComponent: layout
    }

    Text {
        id: hiddenText
    }

    Component {
        id: button
        Compat.StyledButton {
            text: csdata.stateStrings[index]
            foregroundColor: (colorMode == ColorMode.Alarm && !Utils.inPuppet) ? root.alarmColor : root.foreground
            backgroundColor: root.background
            fontSizeMode: root.fontSizeMode
            // bind font properties
            font.bold: hiddenText.font.bold
            font.capitalization: hiddenText.font.capitalization
            font.family: hiddenText.font.family
            //font.hintingPreference: hiddenText.font.hintingPreference // Qt 5.9+ only
            font.italic: hiddenText.font.italic
            font.letterSpacing: hiddenText.font.letterSpacing
            font.pixelSize: hiddenText.font.pixelSize
            font.strikeout: hiddenText.font.strikeout
            font.styleName: hiddenText.font.styleName
            font.underline: hiddenText.font.underline
            font.weight: hiddenText.font.weight
            font.wordSpacing: hiddenText.font.wordSpacing
            checkable: true
            exclusiveGroup: radioInputGroup
            Layout.fillWidth: true
            Layout.fillHeight: true
            checked: index === csdata.value
            onClicked: csdata.setValue(index)
        }
    }

    Component {
        id: layout
        GridLayout {
            columns: stacking == Stacking.Column ? 1 :
                     stacking == Stacking.Row ? csdata.stateStrings.length :
                                                Math.max(2, Math.ceil(Math.sqrt(csdata.stateStrings.length)))

            rowSpacing: 0
            columnSpacing: 0
            Repeater {
                model: csdata.stateStrings.length
                delegate: button
            }
        }
    }
}
