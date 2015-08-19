import QtQuick 2.0
import QtQuick.Layouts 1.0

import PvComponents 1.0

Item {
    id: root
    property real value: 0
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property alias background: panel.color
    property alias foreground : progress.color
    property int  direction: Direction.Right
    property int fillMode: 0 // edge center
    property bool showRange: true



    StyledAxis {
        id: range
        direction: root.direction
        visible: showRange
        anchors.top: (direction == Direction.Left || direction == Direction.Right) ? root.top : undefined
        anchors.left: (direction ==  Direction.Left || direction == Direction.Right) ? undefined : root.left
        width: (direction == Direction.Left || direction == Direction.Right) ? root.width : (range.visible ? root.width / 2 : 0)
        height: (direction == Direction.Up || direction == Direction.Down) ? root.height : (range.visible ? root.height / 2 : 0)
        minimumValue: root.minimumValue
        maximumValue: root.maximumValue

    }
    Rectangle {
        id: panel
        anchors.top: (direction == Direction.Left || direction == Direction.Right) ? (range.visible ? range.bottom : root.top) : root.top
        anchors.left: (direction == Direction.Left || direction == Direction.Right) ? root.left : (range.visible ? range.right : root.left)
        width: (direction == Direction.Left || direction == Direction.Right) ? root.width : range.visible ? root.width / 2 : root.width
        height: (direction == Direction.Up || direction == Direction.Down) ? root.height : range.visible ? root.height / 2 : root.height
        border.width: 2
    }

    Rectangle {
        id: progress
        width: direction == Direction.Up || direction == Direction.Down ? panel.width : panel.width * calcPercentage()
        height: direction == Direction.Left || direction == Direction.Right ? panel.height : panel.height * calcPercentage()
    }

    onDirectionChanged: defineAnchors()
    onValueChanged: defineAnchors()

    function defineAnchors() {
        var middle = (maximumValue - minimumValue) /2
        switch (direction) {
        case Direction.Right:
            if (fillMode == 1)
                if (value > middle) {
                    progress.anchors.right = undefined
                    progress.anchors.left = panel.horizontalCenter
                }
                else {
                    progress.anchors.left = undefined
                    progress.anchors.right = panel.horizontalCenter
                }
            else {
                progress.anchors.right = undefined
                progress.anchors.left = panel.left
            }
            progress.anchors.top = panel.top
            break;
        case Direction.Left:
            if (fillMode == 1)
                if (value > middle) {
                    progress.anchors.left = undefined
                    progress.anchors.right = panel.horizontalCenter
                } else {
                    progress.anchors.right = undefined
                    progress.anchors.left = panel.horizontalCenter
                }
            else {
                progress.anchors.left = undefined
                progress.anchors.right = panel.right
            }
            progress.anchors.top = panel.top
            break;
        case Direction.Up:
            if (fillMode == 1)
                if (value > middle) {
                    progress.anchors.top = undefined
                    progress.anchors.bottom = panel.verticalCenter
                } else {
                    progress.anchors.bottom = undefined
                    progress.anchors.top = panel.verticalCenter
                }
            else {
                progress.anchors.top = undefined
                progress.anchors.bottom = panel.bottom
            }
            progress.anchors.left = panel.left
           break;
        case Direction.Down:
            if (fillMode == 1)
                if (value > middle) {
                    progress.anchors.bottom = undefined
                    progress.anchors.top = panel.verticalCenter
                } else {
                    progress.anchors.top = undefined
                    progress.anchors.bottom = panel.verticalCenter
                }
            else {
                progress.anchors.bottom = undefined
                progress.anchors.top = panel.top
            }
            progress.anchors.left = panel.left
           break;
        }
    }

    function calcPercentage() {
        if (fillMode == 0)
            return Math.min(1.0, (value - minimumValue) / (maximumValue - minimumValue))
        else {
            var middle = (maximumValue - minimumValue) / 2
            return Math.min(1.0, Math.abs(value - middle) / (maximumValue - minimumValue))
        }
    }
}
