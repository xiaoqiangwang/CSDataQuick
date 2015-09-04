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
    property bool showRange: false

    Item {
        id: range
        width: (direction == 0 || direction == 2) ? root.width : root.width / 3
        height: (direction == 0 || direction == 2) ? root.height / 3 : root.height
        anchors.top: (direction == 0 || direction == 2) ? root.top : undefined
        anchors.left: (direction == 0 || direction == 2) ? undefined : root.left

        visible: root.showRange
        Text {
            height: (direction == 0 || direction == 2) ? range.height : 8
            width:  (direction == 0 || direction == 2) ? 10 : range.width
            text: minimumValue
            font.pixelSize: height
            anchors.left: (direction == 0 || direction == 2) ? range.left : undefined
            anchors.top: (direction == 1 || direction == 3) ? range.top : undefined
        }
        Text {
            height: (direction == 0 || direction == 2) ? range.height : 8
            width:  (direction == 0 || direction == 2) ? 10 : range.width
            text: value
            font.pixelSize: height
            anchors.horizontalCenter: (direction == 0 || direction == 2) ? range.horizontalCenter : undefined
            anchors.verticalCenter: (direction == 1 || direction == 3) ? range.verticalCenter : undefined
        }
        Text {
            height: (direction == 0 || direction == 2) ? range.height : 8
            width:  (direction == 0 || direction == 2) ? 10 : range.width
            text: maximumValue
            font.pixelSize: height
            anchors.right: (direction == 0 || direction == 2) ? range.right : undefined
            anchors.bottom: (direction == 1 || direction == 3) ? range.bottom : undefined
        }
    }

    Rectangle {
        id: panel
        anchors.top: (direction == 0 || direction == 2) ? (range.visible ? range.bottom : root.top) : undefined
        anchors.left: (direction == 0 || direction == 2) ? undefined : (range.visible ? range.right : root.left)
        //width: root.width
        //height: root.height * 0.5
        width: (direction == 0 || direction == 2) ? root.width : range.visible ? root.width / 2 : root.width
        height: (direction == 1 || direction == 3) ? root.height : range.visible ? root.height / 2 : root.height
        border.width: 1
    }

    Rectangle {
        id: progress
        width: direction == 1 || direction == 3 ? panel.width : panel.width * calcPercentage()
        height: direction == 0 || direction == 2 ? panel.height : panel.height * calcPercentage()
    }

    onValueChanged: defineAnchors()

    function defineAnchors() {
        var middle = (maximumValue - minimumValue) /2
        switch (direction) {
        case 0:
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
        case 2:
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
        case 1:
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
        case 3:
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
                progress.anchors.top = panel.bottom
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
