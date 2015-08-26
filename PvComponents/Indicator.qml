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
        foreground: root.foreground
    }
    Rectangle {
        id: panel
        anchors.top: (orientation == Qt.Horizontal && range.visible) ? range.bottom : root.top
        anchors.left: (orientation == Qt.Vertical && range.visible) ? range.right : root.left
        anchors.right: root.right
        anchors.bottom: root.bottom

        anchors.leftMargin: (orientation == Qt.Horizontal && range.visible) ? range.sidemargin - 1 : 0
        anchors.rightMargin: anchors.leftMargin
        anchors.topMargin: (orientation == Qt.Vertical && range.visible) ? range.sidemargin - 1 : 0
        anchors.bottomMargin: anchors.topMargin

        border.width: 1
        color: root.background

        // clip indicator when it is near the edge
        clip: true

        Rectangle {
            id: indicator

            x: {
                switch(direction) {
                case Direction.Right:
                    return panel.width * calcPercentage() - width / 2;
                case Direction.Left:
                    return panel.width * (1 - calcPercentage()) - width / 2
                default:
                    return panel.width / 2 - width / 2
                }
            }

            y: {
                switch(direction) {
                case Direction.Up:
                    return panel.height * (1 - calcPercentage()) - height / 2
                case Direction.Down:
                    return panel.height * calcPercentage() - height / 2
                default:
                    return panel.height / 2 - height / 2
                }
            }

            width: (orientation == Qt.Horizontal ? panel.height : panel.width) / Math.sqrt(2)
            height: width

            transform: [

                Rotation {
                    origin.x: indicator.width / 2
                    origin.y: indicator.height / 2
                    angle: 45
                },

                Scale {
                    origin.x: indicator.width / 2
                    origin.y: indicator.height / 2
                    xScale: orientation == Qt.Horizontal ? Math.max(5, panel.width / 10) / indicator.width : 1
                    yScale: orientation == Qt.Horizontal ? 1 : Math.max(5, panel.height / 10) / indicator.height
                }
            ]
            color: root.indicatorColor
        }
    }
    function calcPercentage() {
        return Math.min(1.0, (value - minimumValue) / (maximumValue - minimumValue))
    }
}
