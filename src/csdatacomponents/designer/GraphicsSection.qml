import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Style")

    SectionLayout {
        Label {
            text: qsTr("fill")
            tooltip: qsTr("graphics fill style")
        }
        SecondColumnLayout {
            ComboBox {
                backendValue: backendValues.fill
                implicitWidth: 180
                scope: "FillStyle"
                model: ["Solid", "Outline"]
            }
        }
        Label {
            text: qsTr("edge")
            tooltip: qsTr("graphics edge style")
        }
        SecondColumnLayout {
            ComboBox {
                backendValue: backendValues.edge
                implicitWidth: 180
                scope: "EdgeStyle"
                model: ["Solid", "Dash"]
            }
        }
        Label {
            text: qsTr("line width")
            tooltip: qsTr("line width")
        }
        SecondColumnLayout {
            SpinBox {
                backendValue: backendValues.lineWidth
            }
        }
    }
}


