import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0


/*!
    \qmltype CaShellCommand
    \inqmlmodule PvComponents
    \brief Runs shell commands

    It is a menu button that usually has a graphic consisting of an “!” and a label.
    The menu items denote the commands that can be executed.
    If there is only one command specified in the menu, then the “!” and/or label appear in the middle of the button,
    and the command is activated immediately without bringing up a menu.
    It is not activated until the button is released, so you can depress the button to check
    if there is more than one menu item or not, then abort by releasing the button away from the Shell Command.
    If there is more than one item on the menu, the “!” and/or label are at the left of the button.

    If the command contains a “?”, then the rest of the command will be ignored,
    and a dialog box will prompt the user to complete (or otherwise edit) the command.
*/

CaControl {
    id: shell
    /*!
        text on button

        The Label is the optional label that appears on the button.
        Normally, whether there is a label or not, there is a “!” symbol on the button to denote a Shell Command control.
        However, if the label text begins with  "-" (for example "-My Label"), the “!” symbol will not appear.
    */
    property string label: ''
    /*!
        command list model

        Each ListElement contains three properties, label, command and args.
        The label is the label on the menu item in the menu that is brought up when the Shell Command button is pressed.
        The string in command will be concatenated with the string in args with a space in between,
        and the resulting string will be executed as a system command when the menu item with that label is selected.
        (The args are actually superfluous, since any command arguments can just as well be included in command.)
        The GUI will block until the command is executed, so it is almost always wise to include an “&” at the end of the command
        (or the args if they re used) so that it will execute in the background.
    */
    property ListModel model: ListModel {
    }

    /*!
        \internal
    */
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
        var btnCmd = 'import QtQuick 2.1;' +
                                     'import QtQuick.Controls 1.0;' +
                                     'import PvComponents 1.0;' +
                                     'StyledButton {' +
                                         'anchors.fill: parent;' +
                                         'text: "%1";'.arg(btnLabel) +
                                         '%1\n' +
                                         'foreground: shell.foreground;' +
                                         'background: shell.background;' +
                                         'pixelSize: shell.fontSize;' +
                                         'fontFamily: shell.fontFamily;}'
        // Single entry, direct action on button click
        if (model.count == 1) {
            var btn = Qt.createQmlObject(btnCmd.arg('onClicked: runCommand("%1 %2")'.arg(model.get(0).command).arg(model.get(0).args)), shell, 'button')
            btn.align = Text.AlignHCenter
        // Multiple entries, popup menu on button click
        } else {
            var btn = Qt.createQmlObject(btnCmd.arg('menu: Menu{}'), shell, 'button')
            btn.align = Text.AlignLeft
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