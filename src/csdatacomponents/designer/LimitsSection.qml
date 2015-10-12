import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Limits")

    SectionLayout {
        Label {
            text: qsTr("lower")
            tooltip: qsTr("lower operation limits")
        }
        SecondColumnLayout {
            ComboBox {
                id: loprSrc
                backendValue: backendValues._loprSrc
                scope: "LimitsSource"
                model: ["Channel", "Default", "User"]
            }
            SpinBox {
                backendValue: backendValues._loprDefault
                minimumValue: -Infinity
                maximumValue: Infinity
                decimals: backendValues._prec
                enabled: loprSrc.currentIndex == 1
            }
        }
        Label {
            text: qsTr("upper")
            tooltip: qsTr("upper operation limits")
        }
        SecondColumnLayout {
            ComboBox {
                id: hoprSrc
                backendValue: backendValues._hoprSrc
                scope: "LimitsSource"
                model: ["Channel", "Default", "User"]
            }
            SpinBox {
                backendValue: backendValues._hoprDefault
                minimumValue: -Infinity
                maximumValue: Infinity
                decimals: backendValues._prec
                enabled: hoprSrc.currentIndex == 1
            }
        }
        Label {
            text: qsTr("precision")
            tooltip: qsTr("precision for float number")
        }
        SecondColumnLayout {
            ComboBox {
                id: precSrc
                backendValue: backendValues._precSrc
                scope: "LimitsSource"
                model: ["Channel", "Default", "User"]
            }
            SpinBox {
                backendValue: backendValues._precDefault
                minimumValue: 0
                maximumValue: 25
                enabled: precSrc.currentIndex == 1
            }
        }
    }
}

