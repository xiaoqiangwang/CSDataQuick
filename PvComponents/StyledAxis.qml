import QtQuick 2.0

import PvComponents 1.0

Rectangle {
    id: root

    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property int precision: 1
    property alias font: hiddenText.font
    property int direction: Direction.Right
    property color foreground
    property color background

    readonly property int orientation: (direction == Direction.Left || direction == Direction.Right) ? Qt.Horizontal : Qt.Vertical
    readonly property string fontString: '%1px "%2"'.arg(font.pixelSize).arg(font.family)
    readonly property int sidemargin: Math.min(font.pixelSize, 20)
    readonly property int tickLength: orientation == Qt.Horizontal
                                      ? Math.min((height - font.pixelSize), height / 3)
                                      : Math.min(width - hiddenText.paintedWidth, width / 3)
    implicitWidth: orientation == Qt.Horizontal ?  100 : hiddenText.paintedWidth + font.pixelSize
    implicitHeight: orientation == Qt.Horizontal ? font.pixelSize * 2: 100

    color: background

    onDirectionChanged: canvas.requestPaint()
    onVisibleChanged: canvas.requestPaint()

    Text {
        id: hiddenText
        anchors.fill: parent
        z: -1
        text: {
            var t1 = minimumValue.toFixed(precision);
            var t2 = maximumValue.toFixed(precision);
            return t1.length > t2.length ? t1 : t2
        }
    }

    QtObject {
        id: d
        property int basemargin: 1
    }

    Canvas {
        id: canvas

        anchors.fill: parent

        onPaint: {
            var ctx = getContext('2d')
            ctx.save()

            ctx.clearRect(0, 0, width, height)

            // calculate the label width
            ctx.font = fontString;
            var t1 = minimumValue.toFixed(precision)
            var t2 = maximumValue.toFixed(precision)

            var w1 = ctx.measureText(t1).width
            var w2 = ctx.measureText(t2).width

            // baseline
            var lineWidth = 1
            ctx.lineWidth = lineWidth
            ctx.strokeStyle = foreground
            ctx.beginPath()
            if (direction == Direction.Up || direction == Direction.Down) {

                ctx.moveTo(width - d.basemargin, sidemargin)
                ctx.lineTo(width - d.basemargin, height - sidemargin)
                ctx.stroke()
            }

            if (direction == Direction.Left || direction == Direction.Right) {
                ctx.moveTo(sidemargin, height - d.basemargin)
                ctx.lineTo(width - sidemargin, height - d.basemargin)
                ctx.stroke()
            }

            // ticks
            ctx.beginPath()
            if (direction == Direction.Up || direction == Direction.Down) {
                for (var i=0; i<=10; i++) {
                    var tick = i * (height - 2 * sidemargin)/ 10.

                    ctx.moveTo(width - d.basemargin, sidemargin + tick)
                    ctx.lineTo((i==0 || i==5 || i==10) ? (width - d.basemargin - tickLength) : (width - d.basemargin - 2*tickLength/3), sidemargin + tick)
                }
            }
            if (direction == Direction.Left || direction == Direction.Right) {
                for (var i=0; i<=10; i++) {
                    var tick = i * (width - 2 * sidemargin)/ 10.

                    ctx.moveTo(sidemargin + tick, height - d.basemargin)
                    ctx.lineTo(sidemargin + tick, (i==0 || i==5 || i==10) ? (height - d.basemargin - tickLength) : (height - d.basemargin - 2*tickLength/3))
                }
            }
            ctx.stroke()

            // min max value label
            ctx.font = fontString;
            ctx.fillStyle = foreground

            if (direction == Direction.Up || direction == Direction.Down) {
                ctx.textBaseline = 'middle'
                ctx.textAlign = 'right'
                ctx.fillText(direction == Direction.Down ? t1 : t2, 2 * width / 3, sidemargin)
                ctx.fillText(direction == Direction.Down ? t2 : t1, 2 * width / 3, height - sidemargin)
            }

            if (direction == Direction.Left || direction == Direction.Right) {
                ctx.textBaseline = 'bottom'

                ctx.textAlign =  sidemargin > w1 / 2 ? 'center' : 'left'
                ctx.fillText(direction == Direction.Right ? t1 : t2,  0, height / 2)

                ctx.textAlign =  sidemargin > w2 / 2 ? 'center' : 'right'
                ctx.fillText(direction == Direction.Right ? t2 : t1, width, height / 2)
            }


            ctx.restore()
        }
    }
}
