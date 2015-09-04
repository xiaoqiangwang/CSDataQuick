import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

FocusScope {
    id: root

    property real value: 0.5
    property int precision: 2
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property real stepSize: 1.0

    property color background: 'white'
    property color foreground : 'black'
    property color indicatorColor: 'black'
    property alias font: hiddenText.font

    property int  direction: Direction.Right
    property bool showRange: true

    Text {
        id: hiddenText
        visible: false
        text: {
            var t1 = minimumValue.toFixed(precision);
            var t2 = maximumValue.toFixed(precision);
            return t1.length > t2.length ? t1 : t2
        }
        font.pixelSize: 20
        font.family: 'monospace'
    }

    readonly property int orientation: (direction == Direction.Left || direction == Direction.Right)
                                       ? Qt.Horizontal : Qt.Vertical
    QtObject {
        id: d

        property real markerHeight: 30
        property real rangeWidth: 30

        readonly property int tickmarkCount: (maximumValue - minimumValue) / stepSize + 1
        property real tickmarkWidth: 2
        property real tickmarkLength: hiddenText.height * 0.65
    }

    Item {
        id: container

        width: orientation === Qt.Vertical ? parent.width : parent.height
        height: orientation === Qt.Vertical ? parent.height : parent.width
        rotation: orientation === Qt.Horizontal ? 90 : 0
        transformOrigin: Item.Center
        anchors.centerIn: parent

        Rectangle {
            id: tickmarkLabelBounds
            x: 0
            width: d.tickmarkLength + (orientation == Qt.Vertical ? hiddenText.width : hiddenText.height)
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height

            Item {
                id: tickmarkLabel
                x: 0
                width: hiddenText.width
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    x: 0
                    y: d.markerHeight / 2 - hiddenText.height / 2
                    text: maximumValue.toFixed(precision)
                    color: foreground
                    font: hiddenText.font
                    transformOrigin: Item.Center
                    rotation: orientation == Qt.Vertical ? 0 : -90
                }

                Text {
                    x: 0
                    y: parent.height - d.markerHeight / 2 - hiddenText.height / 2
                    text: minimumValue.toFixed(precision)
                    color: foreground
                    font: hiddenText.font
                    transformOrigin: Item.Center
                    rotation: orientation == Qt.Vertical ? 0 : -90
                }

            }

            Column {
                id: tickmarkColumn
                x: tickmarkLabel.width
                width: d.tickmarkLength
                anchors.verticalCenter: parent.verticalCenter

                spacing: (parent.height - d.markerHeight - d.tickmarkCount * d.tickmarkWidth) / (d.tickmarkCount - 1)

                Repeater {
                    model: d.tickmarkCount
                    delegate: Rectangle {
                        implicitWidth: d.tickmarkLength
                        implicitHeight: d.tickmarkWidth

                        color: 'red'
                    }
                    onHeightChanged: console.log(height)
                }
            }

        }


        Rectangle {
            id: groove
            anchors.left: tickmarkLabelBounds.right
            anchors.verticalCenter: tickmarkLabelBounds.verticalCenter
            height: parent.height
            width: parent.width - tickmarkLabelBounds.width
            color: Qt.darker(background)
            border.width: focus ? 1 : 0
        }

        Rectangle {
            id: handle

            width: groove.width - 6
            anchors.horizontalCenter: groove.horizontalCenter

            y: groove.height * calcPercentage()
            height: d.markerHeight

            color: background
            Frame {
                id: marker
                height:  2
                width: handle.width - 2
                anchors.centerIn: handle
                color: Qt.darker(background)
            }
            MouseArea {
                anchors.fill: parent
                drag {
                    target: handle
                    axis: Drag.YAxis
                    minimumY: 0
                    maximumY: groove.height - handle.height
                }
            }
        }
    }

    function calcPercentage() {
        return Math.min(1.0, (value - minimumValue) / (maximumValue - minimumValue))
    }
}
