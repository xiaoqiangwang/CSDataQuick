import QtQuick 2.1
import QtQuick.Controls 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

MouseArea {
    acceptedButtons: Qt.RightButton
    onReleased: contextMenu.popup()

    onWindowChanged: {
        if (window)
            WindowManager.appendWindow(window)
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: 'Print'
            onTriggered: {
                WindowManager.printWindow(Utils.parentWindow(parent))
            }
        }
        MenuItem {
            text: 'Close'
            onTriggered: {
                WindowManager.closeWindow(Utils.parentWindow(parent))
            }
        }
        MenuItem {
            text: 'MainWindow'
            onTriggered: {
                var mainWindow  = WindowManager.mainWindow
                if (mainWindow) {
                    mainWindow.raise()
                    mainWindow.requestActivate()
                }
            }
        }
        MenuItem {
            text: 'Display List'
            onTriggered: {
                DisplayListDialog.open()
            }
        }
        MenuItem {
            text: 'Data Engines'
            onTriggered: {
                PvTableDialog.open()
            }
        }
    }
}
