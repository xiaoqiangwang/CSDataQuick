import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0
import "utils.js" as Utils


/*!
    \qmltype CaChoiceButton
    \inqmlmodule PvComponents
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
    QtObject {
        id: d
        property var layout: null
    }
    
    ExclusiveGroup { id: radioInputGroup }

    Connections {
        target: pv

        onValueChanged: {
            if (d.layout)
                d.layout.children[pv.value].checked = true
        }
        onConnectionChanged: {
            if (!pv.connected)
                return
            createLayout();
        }
    }

    onStackingChanged: {
        if (pv.connected)
            createLayout()
    }

    /*! \internal */
    function createLayout() {
        // destroy any previous buttons/layout
        if (d.layout) {
            d.layout.destroy();
            d.layout = null
        }
        // calculate font size
        var size = Utils.getBestFontSize(stacking==Stacking.Column ? height / pv.stateStrings.length - 4 : height - 4).size
        // create layout based on stacking
        var cmd = 'import QtQuick 2.0; import QtQuick.Layouts 1.0; import PvComponents 1.0;\n'
        if (stacking == Stacking.Row) {
            cmd += '
                    RowLayout {
                        id: layout;
                        anchors.fill: parent;
                        spacing: 0;
                        enabled: pv.accessRight & CSData.WriteAccess;'
        } else if (stacking == Stacking.Column) {
            cmd += '
                    ColumnLayout {
                        id: layout;
                        anchors.fill: parent;
                        spacing: 0;
                        enabled: pv.accessRight & CSData.WriteAccess;'
        } else {
            cmd += '
                    Flow {
                        id: layout;
                        enabled: pv.accessRight & CSData.WriteAccess;'
        }
        // create buttons
        for(var i = 0; i < pv.stateStrings.length; i++) {
            var name = 'btn'+i
            cmd += '
                    StyledButton {
                        text: "%1";
                        foreground: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground;
                        background: root.background;
                        pixelSize: %3;
                        fontFamily: root.fontFamily;
                        checkable: true;
                        exclusiveGroup: radioInputGroup;
                        Layout.fillWidth: true;
                        Layout.fillHeight: true;
                        onClicked: pv.setValue(%2);
                    }\n'.arg(pv.stateStrings[i]).arg(i).arg(size)
        }
        cmd += '}'
        d.layout = Qt.createQmlObject(cmd, root, 'layout')
    }
}
