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
    function replacer(key, value) {
        if (key == 'objectName')
            return undefined
        else
            return value
    }
    Component {
        id: itemDelegate
        RowLayout {
            width: parent.width
            FieldEditor { 
                width: 80; 
                text: label; 
                onAccepted: {
                    listView.model.setProperty(index, "label", text)
                }
            }
            FieldEditor { 
                Layout.fillWidth: true;
                Layout.minimumWidth: 120; 
                text: command; 
                onAccepted: {
                    listView.model.setProperty(index, "command", text)
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
                listView.model.append({label:'ls %1'.arg(listView.model.count), command:''})
            }
        }
        IconButton {
            iconSource: 'images/apply.png'
            onClicked: {
                var model = []
                for(var i=0; i<listView.model.count; i++) {
                    model.push(listView.model.get(i))
                }
                console.log(JSON.stringify(model, replacer))
                backendValue.expression = JSON.stringify(model, replacer)
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
