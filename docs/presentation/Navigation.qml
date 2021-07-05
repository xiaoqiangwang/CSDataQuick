import QtQuick 2.0
import CSDataQuick.Components 1.0

Row {
    id: root
    height: parent.height * 0.02
    spacing: height

    anchors.bottom: parent.bottom
    anchors.bottomMargin: height
    anchors.horizontalCenter: parent.horizontalCenter

    signal previousPage()
    signal nextPage()

    Repeater {
        model: 2
        Polygon {
            width: 2 * root.height
            height: root.height
            foreground: 'lightGrey'
            fillStyle: FillStyle.Solid
            scale: index ? 1 : -1
            points: [
                Qt.point(0, height/3),
                Qt.point(width/2, height/3),
                Qt.point(width/2, 0),
                Qt.point(width, height/2),
                Qt.point(width/2, height),
                Qt.point(width/2, 2*height/3),
                Qt.point(0, 2*height/3)
            ]
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.foreground = 'grey'
                onExited: parent.foreground = 'lightGrey'
                onClicked: index ? nextPage() : previousPage()
            }
        }
    }
}
