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
        caption: qsTr("Toggle Button")

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
        }
    }

    TextFontSection {}
    ColorSection {}
    DynamicAttributeSection {}
}
