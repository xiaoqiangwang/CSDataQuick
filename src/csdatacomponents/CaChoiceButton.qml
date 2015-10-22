import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS


/*!
    \qmltype CaChoiceButton
    \inqmlmodule CSDataQuick.Components
    \brief Display PV's enumerate

    The Choice Button is used for ENUM process variables and is a means for picking
    the ENUM choices via buttons.

    \qml
    CaChoiceButton {
        source: 'bo'
        stacking: Stacking.Row
    }
    \endqml

    \image choicebutton.png
*/

CaControl {
    id: root

    implicitHeight: stacking == Stacking.Column ? pv.stateStrings.length * 20 : 20
    implicitWidth: (stacking == Stacking.Row || stacking == Stacking.RowColumn) ? pv.stateStrings.length * 100 : 100

    /*!
      Set the layout for the choice buttons. Default is Stacking.Column.

      \list
        \li Stacking.Row: The buttons are arranged in a row
        \li Stacking.Column: The buttons are arranged in a column
        \li Stacking.RowColumn: The buttons are automatically arranged in rows and columns
      \endlist
    */
    property int stacking: Stacking.Column
    /*! \internal */
    property var font: UtilsJS.getBestFontSize(stacking==Stacking.Column ? height / pv.stateStrings.length - 4 : height - 4, true)

    Connections {
        target: pv
        onValueChanged: loader.item.children[pv.value].checked = true
    }

    ExclusiveGroup { id: radioInputGroup }

    Loader {
        id: loader
        anchors.fill: parent
        enabled: pv.accessRight & CSData.WriteAccess
        sourceComponent: (stacking == Stacking.Row || stacking == Stacking.Column) ? layoutStyle : flowStyle
    }

    Component {
        id: button
        StyledButton {
            text: pv.stateStrings[index]
            foreground: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
            background: root.background
            font.pixelSize: root.font.size
            font.family: root.font.family
            checkable: true
            exclusiveGroup: radioInputGroup
            Layout.fillWidth: true
            Layout.fillHeight: true
            onClicked: pv.setValue(index)
        }
    }

    Component {
        id: flowStyle
        Flow {
            spacing: 0
            Repeater {
                model: pv.stateStrings.length
                delegate: button
            }
        }
    }

    Component {
        id: layoutStyle
        GridLayout {
            rows: stacking == Stacking.Row ? 1 : pv.stateStrings.length
            columns: stacking == Stacking.Column ? 1 : pv.stateStrings.length
            rowSpacing: 0
            columnSpacing: 0
            Repeater {
                model: pv.stateStrings.length
                delegate: button
            }
        }
    }
}
