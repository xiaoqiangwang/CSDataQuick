import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2

import PvComponents 1.0

ApplicationWindow
{
    id: root
    property ListModel logModel: ListModel {}

    width: 300
    height: 100
    title: app.applicationName
    visible: true
    color: ColorMap.color4

    menuBar: MenuBar {
        Menu {
            title: 'File'
            MenuItem {
                text: 'Open'
                onTriggered: fileDialog.open()
            }
            MenuItem {
                text: 'Quit'
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: 'View'
            MenuItem {
                text: 'Display List'
                onTriggered: displayDialog.open()
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
                onTriggered: close()
            }
        }
    }

    MessageDialog {
        id: aboutDialog
        title: "About " + app.applicationName
        text: '<h3>%1 %2</h3>An ADL file viewer based on QtQuick'.arg(app.applicationName).arg(app.applicationVersion)
    }

    FileDialog {
        id: fileDialog
        title: "Open file ..."
        onAccepted: {
            createADLDisplay(fileUrl, "", "")
        }
    }

    Dialog {
        id: displayDialog
        modality: Qt.NonModal
        // content item is a window list viewer
        Component {
            id: sectionHeading
            Rectangle {
                width: displayDialog.width
                height: childrenRect.height
                color: "lightsteelblue"
                Text {
                    text: section
                    font.bold: true
                }
            }
        }

        contentItem: ScrollView {
            ListView {
                model: WindowManager.windows
                delegate: Text {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: model.modelData.macro ? model.modelData.macro : '<<No Macro Substitute>>'
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var window = model.modelData
                            window.raise()
                            window.requestActivate()
                        }
                    }
                }
                section.property: "modelData.filePath"
                section.criteria: ViewSection.FullString
                section.delegate: sectionHeading

                onCountChanged: {
                    var newIndex = count - 1 // last index
                    positionViewAtEnd()
                    currentIndex = newIndex
                }
            }
        }
        standardButtons: StandardButton.Ok
    }

    // content item is a log message viewer
    ScrollView {
        anchors.fill: parent

        ListView {
            id: logView
            model: logModel
            delegate: Text {
                width: parent.width
                wrapMode: Text.WordWrap
                text: time + ' ' + message
                color: {
                    switch (type) {
                    case LogLevel.Debug:
                        return "#373737"
                    case LogLevel.Info:
                        return "#0000EE"
                    case LogLevel.Warning:
                        return "#DDDD00"
                    case LogLevel.Critical:
                        return "#CC0000"
                    case LogLevel.Fatal:
                        return "#FFFF00"
                    default:
                        return "#000000"
                    }
                }
            }
            onCountChanged: {
                var newIndex = count - 1 // last index
                positionViewAtEnd()
                currentIndex = newIndex
            }
        }
    }

    onClosing: {
        WindowManager.closeAllWindow()
    }

    // slot called when a new disptach request received
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
        if (absFilePath === '') {
            console.error("Failed to find file ", fileName)
            return
        }

        // if the window exists, bring it up
        var window = WindowManager.findWindow(absFilePath, macro)
        if (window) {
            window.raise()
            window.requestActivate()
            console.info('Activate the already existing display ', absFilePath, macro)
            return
        }

        var qmlCmd = Utils.openADLDisplay(absFilePath, macro)
        window = Utils.createDisplay(qmlCmd, root, absFilePath)
        if (!window) {
            console.error("Failed to create window from ", fileName)
            return
        }

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

        console.info('Open ', absFilePath, macro)
        WindowManager.appendWindow(window, absFilePath, macro)
    }

    function outputMessage(type, message)
    {
        // when the number of log entries exceeds 1000, remove the 100 from the begining
        // but leave the first entry which is the information about the current process.
        if (logModel.count > 1000)
            logModel.remove(1, 100)
        logModel.append({'time': Utils.currentDateTime(), 'type': type, 'message': message})
    }
}

