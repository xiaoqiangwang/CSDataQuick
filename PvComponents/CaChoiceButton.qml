import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0


CaControl {
    id: choiceButtons

    property int orientation: 2

    property var _btnList: []
    property var _layout: null
    
    ExclusiveGroup { id: radioInputGroup }

    Connections {
        target: pv
        onValueChanged: {
            _btnList[pv.value].checked = true
        }
        onConnectionChanged: {
            var i
            if (!pv.connected)
                return
            // destroy any previous buttons/layout
            for(i=0; i<_btnList.length; i++)
                _btnList[i].destroy()
            _btnList = []
            if (_layout) {
                _layout.destroy();
                _layout = null
            }

            // create layout based on orientation
            var cmd = ''
            if (orientation == 0) {
                cmd = 'import QtQuick 2.1;
                                    Row {
                                        id: layout
                                    }'
            } else if (orientation == 1) {
                cmd = 'import QtQuick 2.1;
                                    Column {
                                        id: layout
                                    }'
            } else {
                cmd = 'import QtQuick 2.1; Flow {id: layout}'
            }
            _layout = Qt.createQmlObject(cmd, choiceButtons, 'layout')
            // create buttons
            var w = 0, h = 0;
            for(i = 0; i < pv.nostr; i++) {
                var name = 'btn'+i
                var btn = Qt.createQmlObject('import QtQuick.Controls 1.0;
                                            StyledButton {
                                                    foreground: choiceButtons.foreground
                                                    background: choiceButtons.background
                                                    pixelSize: choiceButtons.fontSize
                                                    fontFamily: choiceButtons.fontFamily
                                                    checkable:true;
                                                    exclusiveGroup: radioInputGroup
                                                    onClicked: pv.setValue(%1)
                                            }'.arg(i), _layout, name)
                btn.text = pv.strs[i]
                _btnList.push(btn)
                if (orientation == 0) {
                    w += btn.width
                    h = Math.max(h, btn.height)
                }
                else {
                    h += btn.height
                    w = Math.max(w, btn.width)
                }
            }
            width = w
            height = h
        }
    }
}
