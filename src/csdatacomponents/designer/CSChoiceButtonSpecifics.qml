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
        caption: qsTr("Choice Button")

        SectionLayout {
            Label {
                text: qsTr("stacking")
                tooltip: qsTr("stacking")
            }
            SecondColumnLayout {
                ComboBoxFix {
                    backendValue: backendValues.stacking
                    implicitWidth: 180
                    scope: "Stacking"
                    model: ["Row", "Column", "RowColumn"]
                }
            }
        }
    }

    ColorSection {}
    DynamicAttributeSection {}
}
