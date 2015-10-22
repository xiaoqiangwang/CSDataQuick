import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Arc")

        SectionLayout {
            Label {
                text: qsTr("begin")
                tooltip: qsTr("begin angle")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.begin
                    implicitWidth: 180
                    minimumValue: -360
                    maximumValue: 360
                }
            }
            Label {
                text: qsTr("span")
                tooltip: qsTr("angle span")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.span
                    implicitWidth: 180
                    minimumValue: -360
                    maximumValue: 360
                }
            }
        }
    }
    GraphicsSection {}
    ColorSection {}
    DynamicAttributeSection {}
}
