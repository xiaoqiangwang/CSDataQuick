import QtQuick 2.0
import QtQuick.Controls 2.2

TextArea {
    id: root

    property color foregroundColor: 'black'
    property color backgroundColor: 'white'

    color: root.foregroundColor
    background: Rectangle {
        color: root.backgroundColor
    }
}
