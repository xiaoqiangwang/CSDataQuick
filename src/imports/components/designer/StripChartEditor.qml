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
            columns: 3
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
            ExpandingSpacer {}
            FieldEditor {
                implicitWidth: 150
                text: model.channel
                placeholderText: 'data source'
                onAccepted: {
                    listView.model.setProperty(index, "channel", text)
                }
            }
            IconButton {
                tooltip: 'remove'
                iconSource: 'images/delete.png'
                onClicked: listView.model.remove(index)
            }
            ExpandingSpacer {}
            /*
            ComboBox {
                id: loprSrc
                style: CustomComboBoxStyle {}
                currentIndex: model.loprSrc
                model: ["Channel", "Default", "User"]
                onCurrentIndexChanged: {
                    listView.model.setProperty(index, "loprSrc", currentIndex)
                }
            }
            SpinBox {
                minimumValue: -Infinity
                maximumValue: Infinity
                value: model.loprDefault
                enabled: loprSrc.currentIndex == 1
                onValueChanged: {
                    listView.model.setProperty(index, "loprDefault", value)
                }
            }
            ComboBox {
                id: hoprSrc
                style: CustomComboBoxStyle {}
                currentIndex: model.hoprSrc
                model: ["Channel", "Default", "User"]
                onCurrentIndexChanged: {
                    listView.model.setProperty(index, "hoprSrc", currentIndex)
                }
            }
            SpinBox {
                minimumValue: -Infinity
                maximumValue: Infinity
                value: model.hoprDefault
                enabled: hoprSrc.currentIndex == 1
                onValueChanged: {
                    listView.model.setProperty(index, "hoprDefault", value)
                }
            }
            */
        }
    }
    Row {
        IconButton {
            iconSource: 'images/add.png'
            onClicked: {
                listView.model.append({'label':'', 'channel':'', 'color':'black',
                                          //'loprSrc': 0, 'loprDefault':0,
                                          //'hoprSrc': 0, 'hoprDefault':0
                                      })
            }
        }
        IconButton {
            iconSource: 'images/apply.png'
            onClicked: {
                var model = []
                for(var i=0; i<listView.model.count; i++) {
                    var m = listView.model.get(i)
                    if (!m.channel)
                        continue
                    model.push({'label':m.label, 'channel':m.channel, 'color':m.color,
                                   //'loprSrc':m.loprSrc, 'loprDefault': m.loprDefault,
                                   //'hoprSrc':m.hoprSrc, 'hoprDefault': m.hoprDefault,
                               })
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
