import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Composite")

        SectionLayout {
            Label {
                text: qsTr("source")
                tooltip: qsTr("file url")
            }
            SecondColumnLayout {
                UrlChooser {
                     Layout.fillWidth: true
                     backendValue: backendValues.source
                     filter: "*.adl *.qml"
                }
            }
            Label {
                text: qsTr("macro")
                tooltip: qsTr("macro expansion")
            }
            LineEdit {
                Layout.fillWidth: true
                backendValue: backendValues.macro
            }
        }
    }
    DynamicAttributeSection {}
}
