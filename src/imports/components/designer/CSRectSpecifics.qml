import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Rectangle")

        SectionLayout {
            Label {
                text: qsTr("radius X")
                tooltip: qsTr("radius X")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.radiusX
                    implicitWidth: 180
                    minimumValue: 0
                    maximumValue: Infinity
                }
            }
            Label {
                text: qsTr("radius Y")
                tooltip: qsTr("radius Y")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.radiusY
                    implicitWidth: 180
                    minimumValue: 0
                    maximumValue: Infinity
                }
            }
        }
    }
    GraphicsSection {}
    ColorSection {}
    DynamicAttributeSection {}
}
