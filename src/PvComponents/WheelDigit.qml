import QtQuick 2.0

import "utils.js" as UtilsJS

Item {
    id: root
    property color foreground
    property string digit: '0'
    property bool plusVisible: true
    property bool minusVisible: true
    property alias font: text.font

    signal plus;
    signal minus;

    implicitWidth: font.pixelSize * 0.6

    Keys.onUpPressed: if(plusVisible) plus()
    Keys.onDownPressed: if(minusVisible) minus()

    Column {
        anchors.fill: parent
        Image {
            x: 0
            width: parent.width
            height: parent.height / 4
            source: root.plusVisible ? (root.activeFocus ? 'images/arrow-up-focus.png':'images/arrow-up.png') : ''
            //fillMode: Image.PreserveAspectFit
            MouseArea {
                id: mau
                anchors.fill: parent
                onClicked: {root.forceActiveFocus(); plus();}
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
            x: 0
            width: parent.width
            height: parent.height / 4
            source: root.minusVisible ? (root.activeFocus ? 'images/arrow-down-focus.png':'images/arrow-down.png') : ''
            //fillMode: Image.PreserveAspectFit
            MouseArea {
                id: mad
                anchors.fill: parent
                onClicked: {root.forceActiveFocus(); minus();}
                enabled: root.minusVisible
            }
        }
    }
}
