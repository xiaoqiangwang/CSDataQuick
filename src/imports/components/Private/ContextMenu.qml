import QtQml 2.1
import QtQuick 2.1

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Compat 1.0 as Compat
import "../utils.js" as UtilsJS

MouseArea {
    id: root

    acceptedButtons: Qt.RightButton
    onReleased: contextMenu.popup()

    Compat.Menu {
        id: contextMenu
        Compat.MenuItem {
            text: 'Print'
            onTriggered: {
                WindowManager.printWindow(Utils.parentWindow(parent))
            }
        }
        Compat.MenuItem {
            text: 'Reload'
            onTriggered: {
                // get current window file path and macro
                var window = Utils.parentWindow(parent)
                var filePath = Utils.getProperty(window, 'filePath')
                var macro = Utils.getProperty(window, 'macro')
                // hide current window
                window.visible = false
                // recreate window from file path and macro
                var newWindow = Utils.createDisplayByFile(root, filePath, macro)
                if (!newWindow) {
                    console.error("Failed to create window from ", filePath)
                    window.visible = true
                    return
                }
                // close current window
                WindowManager.closeWindow(window)
                // show new window
                newWindow.visible = true
                WindowManager.appendWindow(newWindow)
                console.info('Reload ', filePath, macro)
            }
        }
        Compat.MenuItem {
            text: 'Close'
            onTriggered: {
                WindowManager.closeWindow(Utils.parentWindow(parent))
            }
        }
        Compat.MenuItem {
            text: 'MainWindow'
            onTriggered: {
                var mainWindow  = WindowManager.mainWindow
                if (mainWindow) {
                    mainWindow.raise()
                    mainWindow.requestActivate()
                }
            }
        }
        Compat.MenuItem {
            text: 'Display List'
            onTriggered: {
                if (typeof DisplayListDialog.parent !== 'undefined') {
                    DisplayListDialog.parent = root
                }
                DisplayListDialog.open()
            }
        }
        Compat.MenuItem {
            text: 'Data Engines'
            onTriggered: {
                if (typeof DataTableDialog.parent !== 'undefined') {
                    DataTableDialog.parent = root
                }
                DataTableDialog.open()
            }
        }
        Compat.Menu {
            id: executeMenu
            title: 'Execute'

            Instantiator {
                id: executeMenuInst
                model: 0
                delegate: Compat.MenuItem {
                    text: modelData.label || ''
                    onTriggered: runCommand(modelData.command)
                }
                onObjectAdded: executeMenu.insertItem(index, object)
                onObjectRemoved: executeMenu.removeItem(object)
            }
            visible: executeMenuInst.count > 0
        }
    }
    Component.onCompleted: {
        var model = []
        var list = Utils.parseExecList()
        for (var i=0; i<list.length; i++) {
            // If &P is specified, the command is used to address items with PV, skip them in the main list
            if (list[i].command.indexOf('&P') < 0) {
                model.push(list[i])
            }
        }
        if (model.length) {
            executeMenuInst.model = model
        } else {
            if (typeof contextMenu.removeMenu === "function")
                contextMenu.removeMenu(executeMenu)
        }
    }
    function runCommand(command) {
        var window = Utils.parentWindow(parent)
        if (command.indexOf('&?')>=0) {
            var dialog = UtilsJS.popupPromptDialog(root, 'Command', 'command', command)
            dialog.accepted.connect(function(){
                command = command.replace('&?', dialog.input)
                Utils.execute(command)
            })
        } else if (command.indexOf('&A')>=0) {
            command = command.replace('&A', Utils.getProperty(window, 'filePath'))
            Utils.execute(command)
        } else if (command.indexOf('&T')>=0) {
            command = command.replace('&T', Utils.getProperty(window, 'title'))
            Utils.execute(command)
        } else if (command.indexOf('&X')>=0) {
            command = command.replace('&X', Utils.getProperty(window, 'winId'))
            Utils.execute(command)
        } else
            Utils.execute(command)
    }
}
