import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

CaControl {
    id: shell
    property string label: 'Run'
    property ListModel model

    Menu {
        id: __menu
    }

    Component.onCompleted: {
        var btn = Qt.createQmlObject('import QtQuick 2.1; import PvComponents 1.0; StyledButton {text: "%1"; menu: __menu; foreground: shell.foreground; background: shell.background; }'
                                     .arg(shell.label), shell, 'button')
        var label, command, args
        for(var i=0; i<model.count; i++) {
            label = model.get(i).label
            command = model.get(i).command
            args = model.get(i).args
            var action = Qt.createQmlObject('import QtQuick 2.1; import QtQuick.Controls 1.0; import PvComponents 1.0; Action{onTriggered: Utils.execute("%1 %2")}'
                                            .arg(command).arg(args), shell, 'action')
            var item = btn.menu.insertItem(i, label);
            item.action = action
        }
    }
}
