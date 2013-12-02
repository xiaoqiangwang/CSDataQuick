import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

CaControl {
    id: shell
    property string label: ''
    property ListModel model: ListModel {
    }

    function runCommand(command) {
        if (!Utils.execute(command))
            console.error('Error happend when run command: `%1`'.arg(command))
    }

    Component.onCompleted: {
        // Prepend '!' only if label does not starts with '-'
        var btnLabel;
        if(shell.label[0] == '-')
            btnLabel = shell.label.substring(1)
        else
            btnLabel = '! ' + shell.label

        // Create button
        var btn = Qt.createQmlObject('import QtQuick 2.1;' +
                                     'import QtQuick.Controls 1.0;' +
                                     'import PvComponents 1.0;' +
                                     'StyledButton {' +
                                         'anchors.fill: parent;' +
                                         'text: "%1";'.arg(btnLabel) +
                                         'menu: Menu {}\n' +
                                         'foreground: shell.foreground;' +
                                         'background: shell.background;' +
                                         'pixelSize: shell.fontSize;' +
                                         'fontFamily: shell.fontFamily;}',
                                     shell, 'button')

        if (model.count == 1) {
            var action = Qt.createQmlObject('import QtQuick 2.1; import QtQuick.Controls 1.0; import PvComponents 1.0; Action{onTriggered: runCommand("%1 %2")}'
                                        .arg(model.get(0).command).arg(model.get(0).args), shell, 'action')

            btn.action = action
        } else {
            var label, command, args
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                command = model.get(i).command
                args = model.get(i).args
                var action = Qt.createQmlObject('import QtQuick 2.1; import QtQuick.Controls 1.0; import PvComponents 1.0; Action{onTriggered: runCommand("%1 %2")}'
                                            .arg(command).arg(args), shell, 'action')
                var item = btn.menu.insertItem(i, label);
                item.action = action
            }
        }
    }
}
