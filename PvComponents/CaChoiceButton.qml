import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0
import "utils.js" as Utils

CaControl {
    id: choiceButtons

    property int orientation: 0

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
            var size = Utils.getBestFontSize(orientation==1 ? height / pv.nostr - 4 : height - 4)
            // create layout based on orientation
            var cmd = ''
            if (orientation == 0) {
                cmd = 'import QtQuick.Layouts 1.0;
                                    RowLayout {
                                        id: layout
                                        anchors.fill: parent
                                        spacing: 0
                        '
            } else if (orientation == 1) {
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
