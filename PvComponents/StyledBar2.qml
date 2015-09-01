import QtQuick 2.0
import QtQuick.Layouts 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

Item {
    id: root
    property real value: 0
    property int precision: 0
    property real minimumValue: 0.0
    property real maximumValue: 1.0

    property color background: 'white'
    property color foreground : 'black'
    property color indicatorColor: 'black'

    property int  direction: Direction.Right
    property int fillMode: FillMode.FromEdge
    property bool showRange: true
    property alias font: range.font

    readonly property int orientation: (direction == Direction.Left || direction == Direction.Right) ? Qt.Horizontal : Qt.Vertical

    Rectangle {
        anchors.fill: parent
        color: background
    }

    StyledAxis {
        id: range
        anchors.top: root.top
        anchors.left: root.left
        width: orientation == Qt.Horizontal ? root.width : Math.max(root.width / 10, implicitWidth)
        height: orientation == Qt.Horizontal ? Math.max(root.height / 10, implicitHeight) : root.height

        font: root.font
        direction: root.direction
        precision: root.precision
        visible: root.showRange

        minimumValue: root.minimumValue
        maximumValue: root.maximumValue
        background: root.background
    }
    Frame {
        id: panel
        shadow: FrameShadow.Sunken

        anchors.top: (orientation == Qt.Horizontal && range.visible) ? range.bottom : root.top
        anchors.left: (orientation == Qt.Vertical && range.visible) ? range.right : root.left
        anchors.right: root.right
        anchors.bottom: root.bottom

        anchors.leftMargin: (orientation == Qt.Horizontal && range.visible) ? range.sidemargin - 1 : 0
        anchors.rightMargin: anchors.leftMargin
        anchors.topMargin: (orientation == Qt.Vertical && range.visible) ? range.sidemargin - 1 : 0
        anchors.bottomMargin: anchors.topMargin

        color: root.background
    }

    Rectangle {
        id: progress
        width: orientation == Qt.Vertical ? panel.width - 4 : (panel.width - 4) * calcPercentage()
        height: orientation == Qt.Horizontal ? panel.height - 4 : (panel.height - 4) * calcPercentage()
        anchors.margins: 2
        color: root.indicatorColor
    }

    onDirectionChanged: defineAnchors()
    onFillModeChanged: defineAnchors()
    onValueChanged: defineAnchors()

    function defineAnchors() {
        var middle = (maximumValue - minimumValue) /2
        switch (direction) {
        case Direction.Right:
            if (fillMode == FillMode.FromCenter)
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
            if (fillMode == FillMode.FromCenter)
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
            if (fillMode == FillMode.FromCenter)
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
            if (fillMode == FillMode.FromCenter)
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
        if (fillMode == FillMode.FromEdge)
            return Math.min(1.0, (value - minimumValue) / (maximumValue - minimumValue))
        else {
            var middle = (maximumValue - minimumValue) / 2
            return Math.min(1.0, Math.abs(value - middle) / (maximumValue - minimumValue))
        }
    }
}
