import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.1

import PvComponents 1.0

ApplicationWindow
{
    id: root
    width: 300
    height: 100
    title: app.applicationName
    visible: true

    menuBar: MenuBar {
        Menu {
            title: 'File'
            MenuItem {
                text: 'Open'
            }
            MenuItem {
                text: 'Quit'
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: 'Help'
            MenuItem {
                text: 'About'
                onTriggered: aboutDialog.open()
            }

            MenuItem {
                text: 'Quit'
            }
        }

    }

    MessageDialog {
        id: aboutDialog
        title: "About " + app.applicationName
        text: '<h3>%1 %2</h3>An ADL file viewer based on QtQuick'.arg(app.applicationName).arg(app.applicationVersion)
    }

    ListView {
        id: logView
        anchors.fill: parent

        model: logModel
        delegate: Text {
            wrapMode: Text.WordWrap
            text: Date().toString() + '\n' + message
            color: {
                switch (type) {
                case LogLevel.Debug:
                    return "#662219B2"
                case LogLevel.Critical:
                    return "#66FF0000"
                case LogLevel.Warning:
                    return "#66FFFD00"
                case LogLevel.Info:
                    return "#6641DB00"
                }
            }
        }
        onCountChanged: {
            var newIndex = count - 1 // last index
            positionViewAtEnd()
            currentIndex = newIndex
        }

    }

    onClosing: {
        WindowManager.closeAllWindow()
    }

    property ListModel logModel: ListModel {}

    function dispatchRequestReceived(fileName, macro, geometry)
    {
        console.info('File Dispatch Request:\n' +
             '  fileName = ' + fileName + '\n' +
             '  macro = ' + macro + '\n' +
             '  geometry = ' + geometry)
        createADLDisplay(fileName, macro, geometry);
    }

    function createADLDisplay(fileName, macro, geometry)
    {
        // search the file
        var absFilePath = Utils.searchADLFile(fileName, '')
        if (!absFilePath)
            console.error("Failed to find file ", fileName)

        // if the window exists, bring it up
        var window = WindowManager.findWindow(absFilePath, macro)
        if (window) {
            window.raise()
            window.requestActivate()
            return
        }

        var qmlCmd = Utils.openADLDisplay(absFilePath, macro)
        window = Utils.createDisplay(qmlCmd, root, absFilePath)
        if (!window)
            console.error("Failed to create window from ", fileName)

        // set new window geometry based on command line argument "-dg"
        var geometrySpec = Utils.parseX11Geometry(geometry)
        var width = geometrySpec["width"] < 0 ? window.width : geometrySpec["width"]
        var height = geometrySpec["height"]< 0 ? window.height : geometrySpec["height"]
        var xOffset = geometrySpec["xOffset"] < 0 ? window.x : geometrySpec["xOffset"]
        var yOffset = geometrySpec["yOffset"] < 0 ? window.y : geometrySpec["yOffset"]

        // default screen size excluding taskbar, system menus, etc
        var x, y;
        switch(geometrySpec["corner"]) {
        case Qt.TopLeftCorner:
            x = xOffset;
            y = yOffset;
            break;
        case Qt.TopRightCorner:
            x = Screen.desktopAvailableWidth - width - xOffset;
            y = yOffset;
            break;
        case Qt.BottomLeftCorner:
            x = xOffset;
            y = Screen.desktopAvailableHeight - height - yOffset;
            break;
        case Qt.BottomRightCorner:
            x = Screen.desktopAvailableWidth - width - xOffset;
            y = Screen.desktopAvailableHeight - height - yOffset;
            break;
        }
        window.x = x; window.y = y
        window.width = width; window.height = height

        window.macro = macro

        window.visible = true
        window.raise()
        window.requestActivate()

        WindowManager.appendWindow(window, absFilePath, macro)
    }

    function outputMessage(type, message)
    {
        // rotate old logs to be less than 1000
        if (logModel.count > 1000)
            logModel.remove(0, 100)
        logModel.append({'type': type, 'message': message})
    }
}

