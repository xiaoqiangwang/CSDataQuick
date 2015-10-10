import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Trace")
        PolylineEditor {
            width: 100
            height: 100
            backendValue: backendValues.points
        }
    }

    GraphicsSection {}
    ColorSection {}
    DynamicAttributeSection {}
}
