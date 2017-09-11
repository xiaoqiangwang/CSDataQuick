import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Related Display")

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
            Label {
                text: qsTr("visual")
                tooltip: qsTr("visual form")
            }
            SecondColumnLayout {
                ComboBoxFix {
                    backendValue: backendValues.visual
                    implicitWidth: 180
                    scope: "RelatedDisplayVisual"
                    model: ["Menu", "Row", "Column"]
                }
            }
        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Display Entry")

        DisplayEditor {
            backendValue: backendValues.model
        }
    }

    ColorSection {}
}
