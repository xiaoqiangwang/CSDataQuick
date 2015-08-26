import QtQuick 2.0

import "utils.js" as UtilsJS

Item {
    id: root
    property color foreground
    property string digit: '0'
    property real order: 1
    property bool plusVisible: true
    property bool minusVisible: true
    property alias font: text.font

    signal plus;
    signal minus;

    implicitWidth: font.pixelSize * 0.6

    Column {
        anchors.fill: parent
        Image {
            x: parent.width / 4
            width: parent.width / 2
            height: parent.height / 4
            source: root.plusVisible ? (mau.pressed ? 'images/arrow-up-focus.png':'images/arrow-up.png') : ''
            fillMode: Image.Stretch
            MouseArea {
                id: mau
                anchors.fill: parent
                onClicked: plus()
                enabled: root.plusVisible
            }
        }
        Text {
            id: text
            width: parent.width
            height: parent.height / 2
            color: root.foreground
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: digit
        }
        Image {
            x: parent.width / 4
            width: parent.width / 2
            height: parent.height / 4
            source: root.minusVisible ? (mad.pressed ? 'images/arrow-down-focus.png':'images/arrow-down.png') : ''
            fillMode: Image.Stretch
            MouseArea {
                id: mad
                anchors.fill: parent
                onClicked: minus()
                enabled: root.minusVisible
            }
        }
    }
}
