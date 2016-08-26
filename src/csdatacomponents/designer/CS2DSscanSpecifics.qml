import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("2D SScan")

        SectionLayout {
            Label {
                text: qsTr("Scan X")
                tooltip: qsTr("sscan record prefix of X dimension")
            }
            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.scanX
                    showTranslateCheckBox: false
                    implicitWidth: 180
                }
            }
            Label {
                text: qsTr("Scan Y")
                tooltip: qsTr("sscan record prefix of Y dimension")
            }
            SecondColumnLayout {
                LineEdit {
                    backendValue: backendValues.scanY
                    showTranslateCheckBox: false
                    implicitWidth: 180
                }
            }
            Label {
                text: qsTr("Positioner X")
                tooltip: qsTr("Positioner number of X dimension")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.posX
                    implicitWidth: 180
                    minimumValue: 1
                    maximumValue: 4
                }
            }
            Label {
                text: qsTr("Positioner Y")
                tooltip: qsTr("Positioner number of Y dimension")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.posY
                    implicitWidth: 180
                    minimumValue: 1
                    maximumValue: 4
                }
            }
            Label {
                text: qsTr("Detector X")
                tooltip: qsTr("Detector number of X dimension")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.detX
                    implicitWidth: 180
                    minimumValue: 1
                    maximumValue: 70
                }
            }
       }
    }
    ColorSection {}
}
