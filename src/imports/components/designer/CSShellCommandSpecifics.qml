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
        }
    }
    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Command Entry")

        CommandEditor {
            backendValue: backendValues.model
        }
    }

    TextFontSection {}
    ColorSection {}
}
