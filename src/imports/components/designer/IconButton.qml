import QtQuick 2.0
import HelperWidgets 2.0

Item {
    id: iconButton
    implicitWidth: 25
    implicitHeight: 25

    property alias tooltip: toolTipArea.tooltip
    property alias iconSource: image.source
    signal clicked()

   Rectangle {
        anchors.fill: parent
        gradient: Gradient{
            id: pressedGradient
            GradientStop{color: "#444" ; position: 0}
            GradientStop{color: "#333" ; position: 1}
        }
        z: toolTipArea.pressed ? 1 : 0
    }

    Image {
        id: image
        anchors.centerIn: parent
    }
 
    ToolTipArea {
        id: toolTipArea
        anchors.fill: parent
        onClicked: {
            iconButton.clicked()
       }
    }
}


