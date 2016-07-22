import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0

import CSDataQuick.Components 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 400
    height: 400
    title: qsTr("Hello World")
    color: windows_background

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
            }
            MenuItem {
                text: qsTr("&Save")
            }
            MenuItem {
                text: qsTr("&Save As ...")
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: qsTr("Channel")
            MenuItem {
                text: qsTr("Add...")
                onTriggered: tableVisible = true
            }
            MenuItem {
                text: qsTr("Remove...")
                onTriggered: tableVisible = true
            }
        }
    }

    ListModel {
        id: itemModel
    }

    property bool tableVisible: false

    ColumnLayout {
        id: mainLayout
        anchors.margins: 5
        transform: Translate {
            x: tableVisible ? window.width * 0.9 : 0
            Behavior on x { NumberAnimation { duration: 400; easing.type: Easing.OutQuad } }
        }

        Repeater {
            model: itemModel
            RowLayout {
                Text {
                    text: model.channelName
                    width: 120
                }
                Loader {
                    Layout.fillWidth: true
                    sourceComponent: componentForItemType(model.itemType)
                    // so that component can access channelName
                    property string channelName: model.channelName
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        opacity: tableVisible ? 1 : 0
        visible: tableVisible
        Behavior on anchors.left { NumberAnimation { duration: 300 }}
        ColumnLayout {
            TableView {
                id: tableView
                Layout.fillWidth: true
                Layout.fillHeight: true
                TableViewColumn { title: 'Channel'; role: 'channelName'; width: 120}
                TableViewColumn { title: 'Item Type'; role: 'itemType' ; width: 80}
                model: itemModel
            }
            Row {
                TextField { id: channelName }
                ComboBox {
                    id: itemType
                    model: ["Byte", "Bar", "Choice Button", "Menu", "Separator", "Text Entry", "Text Update", "Wheel Switch"]
                }
                Button {
                    text: "Add"
                    onClicked: itemModel.append({'channelName':channelName.text, 'itemType':itemType.currentText})
                }
            }
            Row {
                Button {
                    text: 'Remove Selected'
                    onClicked: itemModel.remove(tableView.currentRow)
                }
                Button {
                    text: 'Done'
                    onClicked: tableVisible = false
                }
            }
        }
    }
    Component { id: bar;         CSBar         {source: channelName}}
    Component { id: bite;        CSByte        {source: channelName}}
    Component { id: choiceButton;CSChoiceButton{source: channelName; stacking: Stacking.Row}}
    Component { id: menu;        CSMenu        {source: channelName}}
    Component { id: separator;   Rectangle     {height: 2}}
    Component { id: textEntry;   CSTextEntry   {source: channelName}}
    Component { id: textUpdate;  CSTextUpdate  {source: channelName}}
    Component { id: wheelSwitch; CSWheelSwitch {source: channelName}}

    function open() {

    }
    function save() {

    }

    function saveAs() {

    }

    function componentForItemType (itemType) {
        if (itemType == 'Byte')
            return bite
        else if (itemType == 'Bar')
            return bar
        else if (itemType == 'Choice Button')
            return choiceButton
        else if (itemType == 'Menu')
            return menu
        else if (itemType == 'Separator')
            return separator
        else if (itemType == "Text Entry")
            return textEntry
        else if (itemType == "Text Update")
            return textUpdate
        else if (itemType == 'Wheel Switch')
            return wheelSwitch
    }
}

