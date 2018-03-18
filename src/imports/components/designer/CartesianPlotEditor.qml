import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import HelperWidgets 2.0

Column {
    id: root
    width: parent.width
    height: listView.height + 50

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
        GridLayout {
            columns: 2
            width: parent.width

            FieldEditor {
                implicitWidth: 150
                text: model.label
                placeholderText: 'trace label'
                onAccepted: {
                    listView.model.setProperty(index, "label", text)
                }
            }
            ColorPalette {
                id: palette
                color: model.color
                Layout.preferredWidth: 30
                Connections {
                    target: palette
                    onColorChanged: {
                        listView.model.setProperty(index, "color", String(palette.color))
                    }
                }
            }

            FieldEditor {
                implicitWidth: 150
                text: model.xchannel
                placeholderText: 'x source'
                onAccepted: {
                    listView.model.setProperty(index, "xchannel", text)
                }
            }
            ExpandingSpacer {
            }

            FieldEditor {
                implicitWidth: 150
                text: model.ychannel
                placeholderText: 'y source'
                onAccepted: {
                    listView.model.setProperty(index, "ychannel", text)
                }
            }
            IconButton {
                tooltip: 'remove'
                iconSource: 'images/delete.png'
                onClicked: listView.model.remove(index)
            }
        }
    }
    Row {
        IconButton {
            iconSource: 'images/add.png'
            onClicked: {
                listView.model.append({'label':'', 'xchannel':'', 'ychannel':'', 'color':'black'})
            }
        }
        IconButton {
            iconSource: 'images/apply.png'
            onClicked: {
                var model = []
                for(var i=0; i<listView.model.count; i++) {
                    var m = listView.model.get(i)
                    if (!m.xchannel && !m.ychannel)
                        continue
                    model.push({'label':m.label, 'xchannel':m.xchannel, 'ychannel':m.ychannel, 'color':m.color})
                }
                backendValue.expression = JSON.stringify(model)
            }
        }
    }
    ListView {
        id: listView
        implicitWidth: parent.width
        implicitHeight: model.count * 26 * 2
        model: ListModel{}
        delegate: itemDelegate 
    }
}
