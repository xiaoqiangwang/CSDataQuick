import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Strip Chart")

        SectionLayout {
            Label {
                text: qsTr("title")
                tooltip: qsTr("graph title")
            }
            LineEdit {
                backendValue: backendValues.title
                showTranslateCheckBox: false
            }
            Label {
                text: qsTr("x label")
                tooltip: qsTr("graph x axis label")
            }
            LineEdit {
                backendValue: backendValues.xLabel
                showTranslateCheckBox: false
            }
            Label {
                text: qsTr("y label")
                tooltip: qsTr("graph y axis label")
            }
            LineEdit {
                backendValue: backendValues.yLabel
                showTranslateCheckBox: false
            }
            Label {
                text: qsTr("period")
                tooltip: qsTr("record total period")
            }
            SecondColumnLayout {
                SpinBox {
                    backendValue: backendValues.period
                    maximumValue: 99999
                }
                ComboBox {
                    backendValue: backendValues.units
                    scope: "TimeUnit"
                    model: ["Second", "Minute"]
                }
             }
        }
    }
    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Strip Chart Entry")

        StripChartEditor {
            backendValue: backendValues.model
        }
    }
    ColorSection {}
}
