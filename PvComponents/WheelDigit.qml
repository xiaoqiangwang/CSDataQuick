import QtQuick 2.0

import "utils.js" as UtilsJS

Rectangle {
    property int value: 0
    property real order: 1

    border.width: focus ? 1 : 0

    Column {
        anchors.fill: parent
        Image {
            x: parent.width / 4
            width: parent.width / 2
            height: parent.height / 4
            source: 'images/arrow-up.png'
            fillMode: Image.Stretch
            visible: text.text != ' '
            MouseArea {
                anchors.fill: parent
                onClicked: console.log(index, 'plus')
            }
        }
        Text {
            id: text
            width: parent.width
            height: parent.height / 2
            font.family: 'Courier'
            font.pixelSize: UtilsJS.getBestFontSize(height)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: value
        }
        Image {
            x: parent.width / 4
            width: parent.width / 2
            height: parent.height / 4
            source: 'images/arrow-down.png'
            fillMode: Image.Stretch
            visible: text.text != ' '
            MouseArea {
                anchors.fill: parent
                onClicked: console.log(index, 'minus')
            }
        }
    }
}
