import QtQuick 2.0
import QtQuick.Layouts 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

Item {
    id: root
    property real value: 0.4
    property int precision: 0
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property real stepSize: 1.0

    property color background: 'white'
    property color foreground : 'black'
    property color indicatorColor: foreground

    property int  direction: Direction.Right
    property bool showRange: true
    property alias font: range.font

    property bool showValueText: false
    property color valueTextColor: foreground

    property bool updateValueWhileDragging: true

    readonly property int orientation: (direction == Direction.Left || direction == Direction.Right) ? Qt.Horizontal : Qt.Vertical

    readonly property real markerWidth: 30
    readonly property real effectiveLength: (orientation == Qt.Horizontal ? width : height) - Math.max(range.sidemargin * 2, markerWidth + 4)

    readonly property real posAtMinimum: {
        switch (direction) {
        case Direction.Up:
            return groove.height - markerWidth - 2;
        case Direction.Right:
            return 2;
        case Direction.Down:
            return 2;
        case Direction.Left:
            return groove.width - markerWidth - 2;
        }
    }

    readonly property real posAtMaximum: {
        switch (direction) {
        case Direction.Up:
            return 2;
        case Direction.Right:
            return groove.width - markerWidth - 2;
        case Direction.Down:
            return groove.height - markerWidth - 2;
        case Direction.Left:
            return 2;
        }
    }

    function posForValue(value) {
        value = Math.max(minimumValue, Math.min(maximumValue, value))
        return (value - minimumValue) * factor * (direction == Direction.Up || direction == Direction.Left ? -1 : 1) + posAtMinimum
    }

    function valueForPos(pos) {
        return (direction == Direction.Up || direction == Direction.Left ? -1 : 1) * (pos - markerWidth / 2 - posAtMinimum) / factor + minimumValue
    }

    readonly property real factor: ((orientation == Qt.Horizontal ? groove.width : groove.height) - markerWidth - 4)
                                   / (maximumValue - minimumValue)

    Rectangle {
        anchors.fill: parent
        color: background
    }

    StyledAxis {
        id: range

        anchors.horizontalCenter: orientation == Qt.Horizontal ?  root.horizontalCenter : undefined
        anchors.verticalCenter: orientation == Qt.Vertical ?  root.verticalCenter : undefined

        width: orientation == Qt.Horizontal ? effectiveLength + 2 * sidemargin : Math.max(root.width / 10, implicitWidth)
        height: orientation == Qt.Horizontal ? Math.max(root.height / 10, implicitHeight) : effectiveLength + 2 * sidemargin

        //font: root.font
        direction: root.direction
        precision: root.precision
        visible: root.showRange

        minimumValue: root.minimumValue
        maximumValue: root.maximumValue
        background: root.background
        foreground: root.foreground
    }

    Text {
        color: valueTextColor
        font: root.font
        text: value.toFixed(precision)
        anchors.top: orientation == Qt.Horizontal ? range.top : undefined
        anchors.left: orientation == Qt.Horizontal ? undefined : range.left
        anchors.horizontalCenter: orientation == Qt.Horizontal ? range.horizontalCenter : undefined
        anchors.verticalCenter: orientation == Qt.Horizontal ? undefined : range.verticalCenter
        visible: showValueText
    }

    Frame {
        id: groove
        shadow: FrameShadow.Sunken

        anchors.left: orientation == Qt.Vertical ? (range.visible ? range.right : root.left) : undefined
        anchors.top: orientation == Qt.Horizontal ? (range.visible ? range.bottom : root.top) : undefined

        anchors.horizontalCenter: orientation == Qt.Horizontal ?  root.horizontalCenter : undefined
        anchors.verticalCenter: orientation == Qt.Vertical ?  root.verticalCenter : undefined

        width: orientation == Qt.Horizontal ? effectiveLength + markerWidth + 4: root.width - range.visible * range.width
        height: orientation == Qt.Horizontal ? root.height - range.visible * range.height : effectiveLength + markerWidth + 4

        border.width: focus ? 1 : 0
        color: Qt.darker(root.background, 1.2)

        Keys.onLeftPressed: {
            if (orientation == Qt.Vertical)
                return
            var inc = (direction == Direction.Left ? 1 : -1)
            inc *= (event.modifiers & Qt.ControlModifier) ? 10 : 1
            value = Math.max(minimumValue, Math.min(maximumValue, value + inc * stepSize))
        }

        Keys.onRightPressed: {
            if (orientation == Qt.Vertical)
                return
            var inc = (direction == Direction.Right ? 1 : -1)
            inc *= (event.modifiers & Qt.ControlModifier) ? 10 : 1
            value = Math.max(minimumValue, Math.min(maximumValue, value + inc * stepSize))
        }

        Keys.onUpPressed: {
            if (orientation == Qt.Horizontal)
                return
            var inc = (direction == Direction.Up ? 1 : -1)
            inc *= (event.modifiers & Qt.ControlModifier) ? 10 : 1
            value = Math.max(minimumValue, Math.min(maximumValue, value + inc * stepSize))
        }

        Keys.onDownPressed: {
            if (orientation == Qt.Horizontal)
                return
            var inc = (direction == Direction.Down ? 1 : -1)
            inc *= (event.modifiers & Qt.ControlModifier) ? 10 : 1
            value = Math.max(minimumValue, Math.min(maximumValue, value + inc * stepSize))
        }
    }

    Frame {
        id: handle
        shadow: FrameShadow.Raise

        x: orientation == Qt.Horizontal ? posForValue(root.value) : 2
        y: orientation == Qt.Horizontal ? 2: posForValue(root.value)

        width: orientation == Qt.Horizontal ? markerWidth : groove.width - 4
        height: orientation == Qt.Horizontal ? groove.height - 4 : markerWidth

        anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : groove.horizontalCenter
        anchors.verticalCenter: orientation == Qt.Horizontal ? groove.verticalCenter : undefined

        color: background
        radius: 2

        Image {
            height: orientation == Qt.Vertical ? 2 : handle.height - 2
            width:  orientation == Qt.Vertical ? handle.width - 2 : 2
            anchors.centerIn: handle
            source: orientation == Qt.Vertical ? "images/groove_horz.png" : "images/groove_vert.png"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: groove
        hoverEnabled: true

        property int clickOffset: 0
        property real pressX: 0
        property real pressY: 0
        property bool handleHovered: false

        function updateHandlePosition(mouse, force) {
            var pos, overThreshold, value
            if (orientation == Qt.Horizontal) {
                pos = mouse.x
                overThreshold = (Math.abs(mouse.x - pressX) >= 2)
                if (overThreshold || force) {
                    value = valueForPos(pos)
                    root.value = Math.min(maximumValue, Math.max(minimumValue, value))
                }
            } else if (orientation == Qt.Vertical) {
                pos = mouse.y
                overThreshold = Math.abs(mouse.y - pressY) >= 2
                if (overThreshold || force) {
                    value = valueForPos(pos)
                    root.value = Math.min(maximumValue, Math.max(minimumValue, value))
                }
            }
        }

        onPositionChanged: {
            if (pressed)
                updateHandlePosition(mouse, false)
        }

        onPressed: {
            groove.forceActiveFocus();
            // Item::contains incorrectly convert QPointF
            // See https://bugreports.qt.io/browse/QTBUG-41452
            var ptToHandle = mapToItem(handle, mouse.x, mouse.y);
            if (handle.contains(Qt.point(ptToHandle.x, ptToHandle.y))) {
                pressX = mouse.x
                pressY = mouse.y
            } else {
                var inc = 1;
                switch(direction) {
                case Direction.Up:
                    inc = ptToHandle.y < 0 ? 1 : -1
                    break
                case Direction.Right:
                    inc = ptToHandle.x < 0 ? -1 : 1
                    break
                case Direction.Down:
                    inc = ptToHandle.y < 0 ? -1 : 1
                    break
                case Direction.Left:
                    inc = ptToHandle.x < 0 ? 1 : -1
                    break;
                }
                if (mouse.modifiers & Qt.ControlModifier)
                    inc *= 10
                value = Math.max(minimumValue, Math.min(maximumValue, value + inc * stepSize))
            }
        }

        onEntered: groove.forceActiveFocus()
        onExited: root.forceActiveFocus()
    }
}
