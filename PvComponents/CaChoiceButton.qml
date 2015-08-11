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

*/

CaControl {
    id: choiceButtons
    /*!
      \list
        \li 0: The buttons are arranged in a row
        \li 1: The buttons are arranged in a column
        \li 2: The buttons are automatically arranged in rows and columns
      \endlist
    */
    property int stacking: 0

    /*! \internal */
    property var _layout: null
    
    ExclusiveGroup { id: radioInputGroup }

    Connections {
        target: pv
        onValueChanged: {
            if (_layout)
                _layout.children[pv.value].checked = true
        }
        onConnectionChanged: {
            var i
            if (!pv.connected)
                return
            // destroy any previous buttons/layout
            if (_layout) {
                _layout.destroy();
                _layout = null
            }
            // calculate font size
            var size = Utils.getBestFontSize(stacking==1 ? height / pv.nostr - 4 : height - 4)
            // create layout based on stacking
            var cmd = ''
            if (stacking == 0) {
                cmd = 'import QtQuick.Layouts 1.0;
                                    RowLayout {
                                        id: layout
                                        anchors.fill: parent
                                        spacing: 0
                        '
            } else if (stacking == 1) {
                cmd = 'import QtQuick.Layouts 1.0;
                                    ColumnLayout {
                                        id: layout
                                        anchors.fill: parent
                                        spacing: 0
                      '
            } else {
                cmd = 'import QtQuick 2.1; Flow {id: layout;'
            }
            // create buttons
            for(i = 0; i < pv.nostr; i++) {
                var name = 'btn'+i
                cmd += 'StyledButton {
                            text: "%1"
                            foreground: choiceButtons.foreground
                            background: choiceButtons.background
                            pixelSize: %3
                            fontFamily: choiceButtons.fontFamily
                            checkable:true;
                            exclusiveGroup: radioInputGroup
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            onClicked: pv.setValue(%2)
                        }'.arg(pv.strs[i]).arg(i).arg(size)
            }
            cmd += '}'
            _layout = Qt.createQmlObject(cmd, choiceButtons, 'layout')
        }
    }
}
