import QtQuick 2.0

Item {
    id: root
    property real value: 0
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property alias background: panel.color
    property alias foreground : progress.color
    property int  direction: 0 // right up left down
    property int fillMode: 0 // edge center

    Rectangle {
        id: panel
        anchors.fill: parent
        border.width: 1
    }

    Rectangle {
        id: progress
        anchors.margins: panel.border.width
        width: direction == 1 || direction == 3 ? parent.width : parent.width * calcPercentage()
        height: direction == 0 || direction == 2 ? parent.height : parent.height * calcPercentage()
    }

    onValueChanged: defineAnchors()

    function defineAnchors() {
        var middle = (maximumValue - minimumValue) /2
        switch (direction) {
        case 0:
            if (fillMode == 1)
                if (value > middle) {
                    progress.anchors.right = undefined
                    progress.anchors.left = root.horizontalCenter
                }
                else {
                    progress.anchors.left = undefined
                    progress.anchors.right = root.horizontalCenter
                }
            else {
                progress.anchors.right = undefined
                progress.anchors.left = root.left
            }
            break;
        case 2:
            if (fillMode == 1)
                if (value > middle) {
                    progress.anchors.left = undefined
                    progress.anchors.right = root.horizontalCenter
                } else {
                    progress.anchors.right = undefined
                    progress.anchors.left = root.horizontalCenter
                }
            else {
                progress.anchors.left = undefined
                progress.anchors.right = root.right
            }
            break;
        case 1:
            if (fillMode == 1)
                if (value > middle) {
                    progress.anchors.top = undefined
                    progress.anchors.bottom = root.verticalCenter
                } else {
                    progress.anchors.bottom = undefined
                    progress.anchors.top = root.verticalCenter
                }
            else {
                progress.anchors.top = undefined
                progress.anchors.bottom = root.bottom
            }
           break;
        case 3:
            if (fillMode == 1)
                if (value > middle) {
                    progress.anchors.bottom = undefined
                    progress.anchors.top = root.verticalCenter
                } else {
                    progress.anchors.top = undefined
                    progress.anchors.bottom = root.verticalCenter
                }
            else {
                progress.anchors.bottom = undefined
                progress.anchors.top = root.bottom
            }
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
