import QtQuick 2.0

Item {
    id: root

    property real value: 0
    property int precision: 0
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property color foreground
    property color background
    property color indicatorColor

    property string title: 'Meter'
    property bool showTitle: false
    property bool showRange: false
    property bool showValue: false

    onValueChanged:  {
        needle.requestPaint()
    }

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
        height: root.height * 0.15
        anchors.top: root.top
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
        font.pixelSize: height
        visible: root.showTitle
    }

    Canvas {
        id: panel
        anchors.top: title.bottom
        anchors.horizontalCenter: root.horizontalCenter
        width: Math.min(root.width, root.height * (1 - 0.15 * showTitle - 0.15 * showRange) * 2)
        height: Math.min(root.width, root.height * (1 - 0.15 * showTitle - 0.15 * showRange) * 2) / 2

        onPaint: {
            var ctx = getContext('2d')

            var lineWidth = 1
            var centery = panel.height
            var centerx = panel.width / 2
            var radius = panel.height - lineWidth
            var angle = Math.PI * Math.min( 1.0, (value - minimumValue) / (maximumValue - minimumValue))

            ctx.strokeStyle = foreground
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
    Canvas {
        id: needle
        anchors.top: title.bottom
        anchors.horizontalCenter: root.horizontalCenter
        width: panel.width
        height: panel.height

        onPaint: {
            var ctx = getContext('2d')
            ctx.save()

            ctx.clearRect(0, 0, width, height)

            var lineWidth = 3
            var centery = panel.height
            var centerx = panel.width / 2
            var radius = panel.height - lineWidth
            var angle = Math.PI * Math.min( 1.0, (value - minimumValue) / (maximumValue - minimumValue))

            ctx.strokeStyle = indicatorColor
            ctx.lineWidth = lineWidth

            // needle
            ctx.beginPath()
            ctx.translate(centerx, centery)
            ctx.rotate(angle)
            ctx.moveTo(-radius, 0)
            ctx.lineTo(0, 0)            
            ctx.stroke()

            ctx.restore()
        }
    }

    Item {
        id: range
        width: panel.width
        height: root.height * 0.15
        anchors.top: panel.bottom
        anchors.horizontalCenter: root.horizontalCenter

        visible: root.showRange
        Text {
            height: range.height
            text: minimumValue.toFixed(precision)
            font.pixelSize: height
            anchors.left: range.left
            color: foreground
        }
        Text {
            height: range.height
            text: maximumValue.toFixed(precision)
            font.pixelSize: height
            anchors.right: range.right
            color: foreground
        }
    }

    Item {
        anchors.bottom: root.bottom
        anchors.horizontalCenter: root.horizontalCenter
        width: panel.width
        height: root.height * 0.15

        visible: root.showValue
        Text {
            anchors.fill: parent
            text: value.toFixed(precision)
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: height
            color: foreground
        }
    }
}
