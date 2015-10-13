import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Controls 1.0 as Controls
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Shell Command")

        SectionLayout {
            Label {
                text: qsTr("label")
                tooltip: qsTr("text label")
            }
            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.label
                    showTranslateCheckBox: false
                    implicitWidth: 180
                }
            }

            Controls.TableView {
                Layout.columnSpan: 2
                Controls.TableViewColumn {
                    role: 'label'
                    title: qsTr("Label")
                }
                Controls.TableViewColumn {
                    role: 'command'
                    title: qsTr("Command")
                }
                model: backendValues.model.value
                onModelChanged: {
                    for (var prop in backendValues.model.complexNode)
                        console.log(prop, backendValues.model.complexNode[prop])
                }
            }

        }
    }
 
    ColorSection {}
}
