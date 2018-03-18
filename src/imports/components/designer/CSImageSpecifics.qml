import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Image")

        SectionLayout {
            Label {
                text: qsTr("source")
                tooltip: qsTr("image url")
            }
            SecondColumnLayout {
                UrlChooser {
                     Layout.fillWidth: true
                     backendValue: backendValues.source
                }
            }
            Label {
                text: qsTr("frame calc")
                tooltip: qsTr("calculate the frame number")
            }
            SecondColumnLayout {
                LineEdit {
                    Layout.fillWidth: true
                    backendValue: backendValues.imageCalc
                }
            }
        }
    }
    DynamicAttributeSection {}
}
