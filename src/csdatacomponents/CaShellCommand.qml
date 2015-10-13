import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaShellCommand
    \inqmlmodule CSDataQuick.Components
    \brief Run shell commands

    It is a menu button that usually has a graphic consisting of an “!” and a \l label.
    The \l model denote the commands that can be executed.
    If there is only one command specified in the menu, then the “!” and/or label appear in the middle of the button,
    and the command is activated immediately without bringing up a menu.
    If there is more than one item on the menu, the “!” and/or label are at the left of the button.

    It is not activated until the button is released, so you can depress the button to check
    if there is more than one menu item or not, then abort by releasing the button away from the Shell Command.

    If the command contains a “?”, then the rest of the command will be ignored,
    and a dialog box will prompt the user to complete (or otherwise edit) the command.

    \qml
    CaShellCommand {
        width: 150
        height: 25
        label: 'Shell Commands'
        model: ListModel {
            ListElement {label: 'ls'; command: 'ls'}
            ListElement {label: 'xterm'; command: 'xterm'}
            ListElement {label: 'run anything'; command: '?';}
        }
    }
    \endqml

    \image shellcommand.png
*/

BaseItem {
    id: root

    background: ColorMap.color51
    foreground: ColorMap.color14
    /*!
        text on button

        The Label is the optional label that appears on the button.
        Normally, whether there is a label or not, there is a “!” symbol on the button to denote a Shell Command control.
        However, if the label text begins with  "-" (for example "-My Label"), the “!” symbol will not appear.
    */
    property string label: ''
    /*!
        command list model

        Each element contains three properties, label, command.
        The label is the label on the menu item in the menu that is brought up when the Shell Command button is pressed.
        The string in command will be executed as a system command when the menu item with that label is selected.

        The GUI will block until the command is executed, so it is almost always wise to include an “&” at the end of the command
        so that it will execute in the background.
    */
    property ListModel model: ListModel {
    }

    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(root.height - 4, true)

    /*!
        \internal
    */
    function parseCommand(command) {
        // if command contains '?', truncate it there and prompt for user completion
        var c = command.indexOf('?')
        if (c >=0) {
            command = command.substring(0, c)
            var dialog = UtilsJS.popupPromptDialog(root, 'Command', 'command', command)
            dialog.accepted.connect(function(){
                runCommand(dialog.input)
            })
        } else
            runCommand(command)
    }

    /*!
        \internal
    */
    function runCommand(command) {
        var status = Utils.execute(command)
        if (!status)
            console.error('Error happend when run command: `%1`'.arg(command))
    }

    Component.onCompleted: {
        // Prepend '!' only if label does not starts with '-'
        var btnLabel;
        if(root.label[0] == '-')
            btnLabel = root.label.substring(1)
        else
            btnLabel = '! ' + root.label

        // Create button
        var btnCmd = 'import QtQuick 2.1;' +
                     'import QtQuick.Controls 1.0;' +
                     'import CSDataQuick.Components.Private 1.0;' +
                      'StyledButton {' +
                        'anchors.fill: parent;' +
                        'text: "%1";'.arg(btnLabel) +
                         '%1\n' +
                         'foreground: root.foreground;' +
                         'background: root.background;' +
                         'font.pixelSize: root.font.size;' +
                         'font.family: root.font.family;}'
        // Single entry, direct action on button click
        if (model.count == 1) {
            var btn = Qt.createQmlObject(btnCmd.arg('onClicked: parseCommand("%1")'.arg(model.get(0).command)), root, 'button')
            btn.align = Text.AlignHCenter
        // Multiple entries, popup menu on button click
        } else {
            var btn = Qt.createQmlObject(btnCmd.arg('menu: Menu{}'), root, 'button')
            btn.align = Text.AlignLeft
            var label, command
            for(var i=0; i<model.count; i++) {
                label = model.get(i).label
                command = model.get(i).command
                var action = Qt.createQmlObject('import QtQuick 2.1; import QtQuick.Controls 1.0; Action{onTriggered: parseCommand("%1")}'
                                            .arg(command), root, 'action')
                var item = btn.menu.insertItem(i, label);
                item.action = action
            }
        }
    }
}
