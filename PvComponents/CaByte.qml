import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0

CaControl {
    property int orientation: Qt.Horizontal
    property int start: 0
    property int end: 31

    property var _comp;

    Component {
        id: horz
        Row {
            Repeater {
                id: rptr
                model: Math.abs(end - start) + 1
                Rectangle{
                    width: parent.width / rptr.count
                    height: parent.height
                    border.width: 1
                }
            }
        }
    }

    Component {
        id: vert
        Column {
            Repeater {
                id: rptr
                model: Math.abs(end - start) + 1
                Rectangle {
                    width: parent.width
                    height: parent.height / rptr.count
                    border.width: 1
                }
            }
        }
    }
    Loader {
        id: loader
        anchors.fill: parent
    }

    Connections {
        target: pv
        onValueChanged: {
            var sign = start < end ? 1 : -1;
            for (var i=0; i<Math.abs(end - start) + 1; i++)
                loader.item.children[i].color = (pv.value & Math.pow(2, start+i*sign)) ? foreground : background
        }
        onConnectionChanged: {
            if (!pv.connected)
                return
            if (orientation == Qt.Horizontal)
                _comp = horz
            else if (orientation == Qt.Vertical)
                _comp = vert
            loader.sourceComponent = _comp
        }

    }
}
