import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Data Source")

    SectionLayout {
        Label {
            text: qsTr("source")
            tooltip: qsTr("data source")
        }
        SecondColumnLayout {
            LineEdit {
                backendValue: backendValues.source
                showTranslateCheckBox: false
                implicitWidth: 180
            }
        }
    }
}

