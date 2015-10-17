import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

/*!
    \qmltype BaseWindow
    \inqmlmodule CSDataQuick.Components
    \brief Base for all toplevel windows

    In addition to \l Window, it records the file path this window represents and the macro expansion.
    Also it implements a context menu,
    \list
    \li Print - capture a screenshot of the current window and send it to printer
    \li Close - close the current window
    \li Main Window - popup the main window if any registered
    \li Display List - popup the window to display windows opened
    \li Data Engines - popup the window to display data engine information
    \endlist

*/
Window {
    id: root
    width: 640
    height: 480
    color: ColorMap.windows_background

    /*! file path this window represents */
    property url filePath
    /*! macro expansion */
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

    MouseArea {
        z: 0
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onReleased: contextMenu.popup()
    }
}

