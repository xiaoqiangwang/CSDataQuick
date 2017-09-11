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
            ComboBoxFix {
                id: loprSrc
                backendValue: backendValues.limits_loprSrc
                scope: "LimitsSource"
                model: ["Channel", "Default", "User"]
            }
            SpinBox {
                backendValue: backendValues.limits_loprDefault
                minimumValue: -Infinity
                maximumValue: Infinity
                decimals: backendValues.limits_prec.value
                enabled: loprSrc.currentIndex == 1
            }
        }
        Label {
            text: qsTr("upper")
            tooltip: qsTr("upper operation limits")
        }
        SecondColumnLayout {
            ComboBoxFix {
                id: hoprSrc
                backendValue: backendValues.limits_hoprSrc
                scope: "LimitsSource"
                model: ["Channel", "Default", "User"]
            }
            SpinBox {
                backendValue: backendValues.limits_hoprDefault
                minimumValue: -Infinity
                maximumValue: Infinity
                decimals: backendValues.limits_prec.value
                enabled: hoprSrc.currentIndex == 1
            }
        }
        Label {
            text: qsTr("precision")
            tooltip: qsTr("precision for float number")
        }
        SecondColumnLayout {
            ComboBoxFix {
                id: precSrc
                backendValue: backendValues.limits_precSrc
                scope: "LimitsSource"
                model: ["Channel", "Default", "User"]
            }
            SpinBox {
                backendValue: backendValues.limits_precDefault
                minimumValue: 0
                maximumValue: 25
                enabled: precSrc.currentIndex == 1
            }
        }
    }
}

