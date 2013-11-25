import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0


CaControl {
    id: display
    property string label: 'More'
    property int visual: 2

    // related display list model
    property ListModel model: ListModel {
        ListElement {
            label : 'A'
            name: '../test.qml'
            policy : 'replace'
        }
        ListElement {
            label : 'B'
            name: 'b'
            policy : 'replace'
        }
    }

    function load(name, policy) {
        var top = Qt.createQmlObject('import QtQuick.Window 2.0; Window {visible: true}', display, 'top')
        top.title = name
        var component = Qt.createComponent(name, top)
        if (component.status == Component.Ready) {
            var win = component.createObject(top)
            if (win == null) {
                console.log('Error to create object', name)
            } else {
                if (policy == 'replace') {
                }
            }
        } else {
            console.log('Error to create object', name)
        }
    }

    Component.onCompleted: {
        var label, name, policy;
        switch (visual) {
        case 0:
            var btn = Qt.createQmlObject('import QtQuick 2.1; import PvComponents 1.0; StyledButton {text: "%1"; menu: Menu{}; background: display.background; foreground: display.foreground}'
                                         .arg(display.label), display, 'button')
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                name = model.get(i).name
                policy = model.get(i).policy
                var action = Qt.createQmlObject('import QtQuick 2.1; import QtQuick.Controls 1.0; Action{onTriggered: load("%1", "%2")}'
                                                .arg(name).arg(policy), display, 'action')
                var item = btn.menu.insertItem(i, label);
                item.action = action
            }
            break;
        case 1:
            var layout = Qt.createQmlObject('import QtQuick 2.1; import QtQuick.Controls 1.0; Row {}', display, 'layout')
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                name = model.get(i).name
                policy = model.get(i).policy
                var btn = Qt.createQmlObject('import QtQuick 2.1; import PvComponnets 1.0; StyldButton{text: "%1";onClicked: load("%2", "%3"); background: display.background; foreground: display.foreground;}'
                                             .arg(label).arg(name).arg(policy), layout, 'button')
            }
            break;
        case 2:
            var layout = Qt.createQmlObject('import QtQuick 2.1; import QtQuick.Controls 1.0; Column {}', display, 'layout')
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                name = model.get(i).name
                policy = model.get(i).policy
                var btn = Qt.createQmlObject('import QtQuick 2.1; import PvComponents 1.0; StyledButton{text: "%1";onClicked: load("%2", "%3"); background: display.background; foreground: display.foreground}'
                                             .arg(label).arg(name).arg(policy), layout, 'button')
            }
            break;
        default:
            break;
        }
    }
}
