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
                text: qsTr("size")
            }
            SecondColumnLayout {
                Label {
                    text: "W"
                    width: 12
                }

                SpinBox {
                    backendValue: backendValues.width
                    minimumValue: 0
                    maximumValue: 10000
                    decimals: 0
                }

                Label {
                    text: "H"
                    width: 12
                }

                SpinBox {
                    backendValue: backendValues.height
                    minimumValue: 0
                    maximumValue: 10000
                    decimals: 0
                }

                ExpandingSpacer {

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
