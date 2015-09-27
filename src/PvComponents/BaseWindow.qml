import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.2

import PvComponents 1.0

Window {
    id: root

    property url filePath
    property string macro

    Menu {
        id: contextMenu
        MenuItem {
            text: 'Print'
            onTriggered: {
                WindowManager.printWindow(root)
            }
        }
        MenuItem {
            text: 'Close'
            onTriggered: {
                WindowManager.closeWindow(root)
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
    }

    MouseArea {
        z: 0
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onReleased: contextMenu.popup()
    }
}

