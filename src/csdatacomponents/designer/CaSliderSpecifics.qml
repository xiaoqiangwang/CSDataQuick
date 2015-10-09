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
        caption: qsTr("Slider")

        SectionLayout {
            Label {
                text: qsTr("direction")
                tooltip: qsTr("incremental direction")
            }
            SecondColumnLayout {
                ComboBox {
                    backendValue: backendValues.direction
                    implicitWidth: 180
                    scope: "Direction"
                    model: ["Up", "Right", "Down", "Left"]
                }
            }
            Label {
                text: qsTr("label style")
                tooltip: qsTr("set the decoration style")
            }
            SecondColumnLayout {
                ComboBox {
                    backendValue: backendValues.label
                    implicitWidth: 180
                    scope: "LabelStyle"
                    model: ["Frame", "None", "Outline", "Limits", "Channel"]
                }
            }
            Label {
                text: qsTr("step")
                tooltip: qsTr("step size")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.stepSize
                    implicitWidth: 180
                }
            }
        }
    }

    LimitsSection {}
    ColorSection {}
    DynamicAttributeSection {}
}
