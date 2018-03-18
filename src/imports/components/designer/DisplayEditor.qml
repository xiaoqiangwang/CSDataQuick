import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0

Column {
    id: root
    width: 300
    height: listView.height + 25

    property var backendValue
    property var valueFromBackend: backendValue.expression

    onBackendValueChanged: {
        updateModel(backendValue.expression)
    }
    onValueFromBackendChanged: {
        updateModel(valueFromBackend)
    }

    function updateModel(expression) {
        var model = JSON.parse(expression)
        listView.model.clear()
        for(var i=0; i<model.length; i++) {
            listView.model.append(model[i]) 
        }
    }
    Component {
        id: itemDelegate
        RowLayout {
            width: parent.width
            FieldEditor { 
                width: 80
                text: label
                placeholderText: 'label'
                onAccepted: {
                    listView.model.setProperty(index, "label", text)
                }
            }
            FieldEditor { 
                Layout.fillWidth: true
                Layout.minimumWidth: 120
                text: file
                placeholderText: 'file name'
                onAccepted: {
                    listView.model.setProperty(index, "file", text)
                }
            }
            FieldEditor {
                Layout.fillWidth: true
                Layout.minimumWidth: 120
                text: macro
                placeholderText: 'macro'
                onAccepted: {
                    listView.model.setProperty(index, "macro", text)
                }
            }
            CheckBox {
                checked: replace
                text: 'replace'
                onClicked: {
                    listView.model.setProperty(index, "replace", checked)
                }
            }

            Row {
                IconButton {
                    iconSource: 'images/arrow_up.png'
                    tooltip: 'move up'
                    onClicked: {
                        console.log(index)
                        if (index > 0)
                            listView.model.move(index, index - 1, 1)
                    }
                }
                IconButton {
                    iconSource: 'images/arrow_down.png'
                    tooltip: 'move down'
                    onClicked: {
                        console.log(index)
                        if (index < listView.model.count - 1)
                            listView.model.move(index, index + 1, 1)
                    }
                }
                IconButton {
                    tooltip: 'remove'
                    iconSource: 'images/delete.png'
                    onClicked: listView.model.remove(index)
                }
            }  
        }
    }
    Row {
        IconButton {
            iconSource: 'images/add.png'
            onClicked: {
                listView.model.append({'label':'', 'file':'', 'macro':'', 'replace':false})
            }
        }
        IconButton {
            iconSource: 'images/apply.png'
            onClicked: {
                var model = []
                for(var i=0; i<listView.model.count; i++) {
                    var m = listView.model.get(i)
                    model.push({'label':m.label, 'file':m.file, 'macro':m.macro, 'replace': m.replace})
                }
                backendValue.expression = JSON.stringify(model)
            }
        }
    }
    ListView {
        id: listView
        implicitWidth: parent.width
        implicitHeight: model.count * 26
        model: ListModel{}
        delegate: itemDelegate 
    }
}
