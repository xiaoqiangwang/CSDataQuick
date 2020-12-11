import QtQuick 2.0

import CSDataQuick.Components 1.0

Item {
    id: root

    property real value: 0
    property int precision: 0
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property color foreground: 'black'
    property color background: 'white'
    property color indicatorColor: 'black'

    property alias font: title.font
    property string title: 'Meter'
    property bool showTitle: false
    property bool showRange: false
    property bool showValue: false

    readonly property real radius: Math.min(root.width / 2, (root.height  - font.pixelSize * (showTitle + showRange + showValue)))

    // rectangle filling the whole area with background color
    Rectangle {
        id: background
        anchors.fill: parent
        color: root.background
    }

    Text {
        id: title
        text: root.title
        width: root.width
        height: font.pixelSize
        anchors.top: root.top
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
        visible: root.showTitle
    }

    Canvas {
        id: panel
        anchors.top: root.showTitle ? title.bottom : root.top
        anchors.horizontalCenter: root.horizontalCenter
        width: radius * 2
        height: radius

        onPaint: {
            var ctx = getContext('2d')

            var lineWidth = 1
            var centery = panel.height
            var centerx = panel.width / 2
            var radius = panel.height - lineWidth
            var angle = Math.PI * Math.min( 1.0, (root.value - root.minimumValue) / (root.maximumValue - root.minimumValue))

            ctx.strokeStyle = root.foreground
            // dial panel
            ctx.save()
            ctx.clearRect(0, 0, panel.width, panel.height)
            ctx.translate(centerx, centery)
            ctx.lineWidth = lineWidth

            // scale arc
            ctx.beginPath()
            ctx.arc(0, 0, radius, 0, -Math.PI, true)
            ctx.stroke()

            // scale marks
            ctx.beginPath()
            for (var i=0; i<=10; i++) {
                ctx.moveTo(-radius, 0)
                ctx.lineTo(-radius * 0.8, 0)
                ctx.rotate(Math.PI / 10.)
            }
            ctx.stroke()
            ctx.restore()
        }
    }

    Rectangle {
        id: needle
        color: root.indicatorColor

        anchors.bottom: panel.bottom
        height: panel.height * 0.7

        anchors.horizontalCenter: panel.horizontalCenter
        width: 2

        rotation: 180 * Math.min( 1.0, (root.value - root.minimumValue) / (root.maximumValue - root.minimumValue)) - 90
        transformOrigin: Item.Bottom

    }

    Item {
        id: range
        width: panel.width
        height: root.font.pixelSize
        anchors.top: panel.bottom
        anchors.horizontalCenter: root.horizontalCenter

        visible: root.showRange
        Text {
            height: range.height
            text: root.minimumValue.toFixed(root.precision)
            font: root.font
            anchors.left: range.left
            color: root.foreground
        }
        Text {
            height: range.height
            text: root.maximumValue.toFixed(root.precision)
            font: root.font
            anchors.right: range.right
            color: root.foreground
        }
    }

    Item {
        id: value_text
        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter
        width: panel.width
        height: root.font.pixelSize

        visible: root.showValue
        Text {
            anchors.fill: parent
            text: root.value.toFixed(root.precision)
            horizontalAlignment: Text.AlignHCenter
            font: root.font
            color: root.foreground
            Rectangle {
                anchors.fill: parent
                color: 'white'
                z: -1
            }
        }
    }
}
