import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Polyline")
        SectionLayout {
            columns: 2
            Label {
                text: qsTr("arrow")
                tooltip: qsTr("arrow position")
            }
            ComboBoxFix {
                backendValue: backendValues.arrowPosition
                implicitWidth: 180
                scope: "ArrowPosition"
                model: ["None", "Start", "End", "Both"]
            }
        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Trace")
        Column {
            Row {
                IconButton {
                    iconSource: 'images/cursor-arrow.png'
                    tooltip: 'Draw'
                    onClicked: editor.mode = 0
                }
                IconButton {
                    iconSource: 'images/cursor-cross.png'
                    tooltip: 'Edit nodes'
                    onClicked: editor.mode = 1
                }
            }
            PolylineEditor {
                id: editor
                width: backendValues.width.value
                height: backendValues.height.value
                backendValue: backendValues.points
                closePath: false
            }
        }
    }

    GraphicsSection {}
    ColorSection {}
    DynamicAttributeSection {}
}
