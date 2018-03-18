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
        caption: qsTr("Byte")

        SectionLayout {
            Label {
                text: qsTr("orientation")
                tooltip: qsTr("orientation")
            }
            SecondColumnLayout {
                ComboBox {
                    backendValue: backendValues.orientation
                    implicitWidth: 180
                    scope: "Qt"
                    model: ["Horizontal", "Vertical"]
                }
            }
            Label {
                text: qsTr("begin bit")
                tooltip: qsTr("bit on the left or top")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.start
                    maximumValue: 31
                }
            }
            Label {
                text: qsTr("end bit")
                tooltip: qsTr("bit on the right or bottom")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.end
                    maximumValue: 31
                }
            }
        }
    }

    ColorSection {}
    DynamicAttributeSection {}
}
