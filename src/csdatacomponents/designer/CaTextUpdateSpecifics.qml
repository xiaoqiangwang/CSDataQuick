import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Controls 1.0 as Controls
import QtQuick.Layouts 1.0

Column {
    DataSourceSection {}

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Text")

        SectionLayout {
            Label {
                text: qsTr("alignment")
                tooltip: qsTr("text horizontal alignment")
            }
            SecondColumnLayout {
                ComboBox {
                    backendValue: backendValues.align
                    implicitWidth: 180
                    scope: "Text"
                    model: ["AlignLeft", "AlignRight", "AlignHCenter", "AlignJustify"]
                }
            }
            Label {
                text: qsTr("precision")
                tooltip: qsTr("numbers after decimal point")
            }
            SecondColumnLayout {
                ComboBox {
                    id: precSrc
                    currentIndex: backendValues.limits_precSrc
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
            Label {
                text: qsTr("format")
                tooltip: qsTr("text format")
            }
            SecondColumnLayout {
                ComboBox {
                    backendValue: backendValues.format
                    implicitWidth: 180
                    scope: "TextFormat"
                    model: ["Decimal", "Exponential", "EngNotation", "Compact", "Truncated", 
                    "Hexadecimal", "Octal", "String", "Sexagesimal", "SexagesimalDMS", "SexagesimalHMS"]
                }
            }
        }
    }
    ColorSection {}
    DynamicAttributeSection {}
}
