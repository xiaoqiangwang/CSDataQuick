import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Dynamic Attribute")

    SectionLayout {
        Label {
            text: qsTr("color mode")
            tooltip: qsTr("color mode")
        }
        SecondColumnLayout {
            ComboBoxFix {
                backendValue: backendValues.colorMode
                implicitWidth: 180
                scope: "ColorMode"
                model: ["Static", "Alarm"]
            }
        }
        Label {
            text: qsTr("alarm mode")
            tooltip: qsTr("alarm mode")
        }
        SecondColumnLayout {
            ComboBoxFix {
                backendValue: backendValues.alarmMode
                implicitWidth: 180
                scope: "AlarmMode"
                model: ["Foreground", "Background"]
            }
        }
        Label {
            text: qsTr("Visibility")
            tooltip: qsTr("visibility mode")
        }
        SecondColumnLayout {
            ComboBoxFix {
                backendValue: backendValues.dynamicAttribute_visibilityMode
                implicitWidth: 180
                scope: "VisibilityMode"
                model: ["Static", "IfNotZero", "IfZero", "Calc"]
            }
        }
        Label {
            text: qsTr("Visibility Calc")
            tooltip: qsTr("visibility calc")
        }
        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.dynamicAttribute_visibilityCalc
                showTranslateCheckBox: false
                implicitWidth: 180
            }
        }
        Label {
            text: qsTr("channel A")
            tooltip: qsTr("channel A")
        }
        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.dynamicAttribute_channel
                showTranslateCheckBox: false
                implicitWidth: 180
            }
        }
        Label {
            text: qsTr("channel B")
            tooltip: qsTr("channel B")
        }
        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.dynamicAttribute_channelB
                showTranslateCheckBox: false
                implicitWidth: 180
            }
        }
        Label {
            text: qsTr("channel C")
            tooltip: qsTr("channel C")
        }
        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.dynamicAttribute_channelC
                showTranslateCheckBox: false
                implicitWidth: 180
            }
        }
        Label {
            text: qsTr("channel D")
            tooltip: qsTr("channel D")
        }
        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.dynamicAttribute_channelD
                showTranslateCheckBox: false
                implicitWidth: 180
            }
        }
    }
}
