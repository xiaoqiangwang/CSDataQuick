import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    DataSourceSection {}

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Message Button")

        SectionLayout {
            Label {
                text: qsTr("text")
                tooltip: qsTr("label on button")
            }
            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.text
                    implicitWidth: 180
                }
            }
            Label {
                text: qsTr("on")
                tooltip: qsTr("message when pressed")
            }
            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.onMessage
                    implicitWidth: 180
                }
            }
            Label {
                text: qsTr("off")
                tooltip: qsTr("message when released")
            }
            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.offMessage
                    implicitWidth: 180
                }
            }
        }
    }

    ColorSection {}
    DynamicAttributeSection {}
}
