import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    DataSourceSection {}

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Text Entry Array")

        SectionLayout {
            Label {
                text: qsTr("index")
                tooltip: qsTr("index of the first element")
            }
            SpinBox {
                backendValue: backendValues.index
                implicitWidth: 180
                minimumValue: 0
                maximumValue: 65536
            }
            Label {
                text: qsTr("count")
                tooltip: qsTr("number of elements")
            }
            SpinBox {
                backendValue: backendValues.count
                implicitWidth: 180
                minimumValue: 1
                maximumValue: 65536
            }
            Label {
                text: qsTr("labels")
                tooltip: qsTr("element label")
            }
            Column {
                LineEdit {
                    backendValue: backendValues.label
                    implicitWidth: 180
                }
                CheckBox {
                    text: 'visible'
                    backendValue: backendValues.labelVisible
                }
            }
        }
    }
    TextFormatSection {}
    ColorSection {}
    DynamicAttributeSection {}
}
