import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Cartesian Plot")

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
                text: qsTr("count source")
                tooltip: qsTr("data count source")
            }
            LineEdit {
                backendValue: backendValues.countPvName
                showTranslateCheckBox: false
            }
        }
    }
    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Cartesian Plot Entry")

        CartesianPlotEditor {
            backendValue: backendValues.model
        }
    }
    ColorSection {}
}
