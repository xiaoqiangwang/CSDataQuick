import QtQuick 2.1
import QtQuick.Controls 1.0
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    DataSourceSection {}

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Text Entry Array")

        SectionLayout {
            Label {
                text: qsTr("index")
                tooltip: qsTr("index of the first element")
            }
            SpinBox {
                backendValue: backendValues.index
                implicitWidth: 180
                minimumValue: 0
                maximumValue: 65536
            }
            Label {
                text: qsTr("count")
                tooltip: qsTr("number of elements")
            }
            SpinBox {
                backendValue: backendValues.count
                implicitWidth: 180
                minimumValue: 1
                maximumValue: 65536
            }
            Label {
                text: qsTr("labels")
                tooltip: qsTr("element label")
            }
            Column {
                FieldEditor {
                    property var backendValue: backendValues.label
                    property var valueFromBackend: backendValue.expression
                    onBackendValueChanged: {
                        updateModel(backendValue.expression)
                    }
                    onValueFromBackendChanged: {
                        updateModel(valueFromBackend)
                    }
                    implicitWidth: 180
                    placeholderText: 'label texts separated by ;'
                    onAccepted: {
                        if (!text) {
                            backendValue.expression = '[]'
                            return
                        }

                        var model = text.split(';')
                        var expression = '['
                        for(var i=0; i<model.length; i++) {
                            expression += '"' + model[i] + '"'
                            if (i != model.length -1)
                                expression += ','
                        }
                        expression += ']'
                        backendValue.expression = expression
                    }
                    function updateModel (expression) {
                        if (!expression) {
                            text = ''
                            return
                        }
                        var model = eval(expression)
                        var s = ''
                        for(var i=0; i<model.length; i++) {
                            s += model[i]
                            if (i != model.length - 1)
                                s += ';'
                        }
                        text = s
                    }
                }
                CheckBox {
                    text: 'visible'
                    backendValue: backendValues.labelVisible
                }
            }
        }
    }
    TextFormatSection {}
    ColorSection {}
    DynamicAttributeSection {}
}
