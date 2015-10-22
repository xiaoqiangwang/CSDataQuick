import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Window")

        SectionLayout {
            Label {
                text: qsTr("title")
                tooltip: qsTr("window title")
            }
            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.title
                    implicitWidth: 180
                }
            }
            Label {
                text: 'background'
            }
            ColorPalette {
                backendValue: backendValues.color
            }
        }
    }
}
