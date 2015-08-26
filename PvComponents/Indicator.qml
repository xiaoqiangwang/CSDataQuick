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
    }

    Rectangle {
        id: progress
        x: orientation == Qt.Horizontal ? panel.x + panel.width * calcPercentage() - width / 2: undefined
        anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : panel.horizontalCenter
        y: orientation == Qt.Horizontal ? undefined: panel.y + panel.height * calcPercentage() - height / 2
        anchors.verticalCenter: orientation == Qt.Horizontal ? panel.verticalCenter : undefined
        width: Math.min(panel.width, panel.height) / Math.sqrt(2)
        height: width

        transform: [

            Rotation {
                origin.x: progress.width / 2
                origin.y: progress.height / 2
                angle: 45
            },

            Scale {
                origin.x: progress.width / 2
                origin.y: progress.height / 2
                xScale: orientation == Qt.Horizontal ? 0.5 : 1
                yScale: orientation == Qt.Horizontal ? 1 : 0.5
            }
        ]

        border.width: 2
        color: root.indicatorColor
    }

    function calcPercentage() {
        return Math.min(1.0, (value - minimumValue) / (maximumValue - minimumValue))
    }
}
