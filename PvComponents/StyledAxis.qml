import QtQuick 2.0

import PvComponents 1.0

Rectangle {
    id: root

    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property int precision: 3
    property string font: '12px monospace'
    property int direction: Direction.Right

    QtObject {
        id: d
        property int basemargin: 3
        property int sidemagin: 5
        property int textmargin: 2
    }

    onDirectionChanged: canvas.requestPaint()

    Canvas {
        id: canvas

        anchors.fill: parent

        onPaint: {
            var ctx = getContext('2d')
            ctx.save()

            ctx.clearRect(0, 0, width, height)

            // calculate the label width
            ctx.font = font;
            var t1 = minimumValue.toFixed(precision)
            var t2 = maximumValue.toFixed(precision)

            var w1 = ctx.measureText(t1).width
            var w2 = ctx.measureText(t2).width

            // baseline
            ctx.beginPath()
            if (direction == Direction.Up || direction == Direction.Down) {

                ctx.moveTo(width - d.basemargin, d.sidemagin)
                ctx.lineTo(width - d.basemargin, height - d.sidemagin)
                ctx.stroke()
            }

            if (direction == Direction.Left || direction == Direction.Right) {
                ctx.moveTo(d.sidemagin, height - d.basemargin)
                ctx.lineTo(width - d.sidemagin, height - d.basemargin)
                ctx.stroke()
            }

            // ticks
            ctx.beginPath()
            if (direction == Direction.Up || direction == Direction.Down) {
                for (var i=0; i<=10; i++) {
                    var tick = i * (height - 2 * d.sidemagin)/ 10.

                    ctx.moveTo(width - d.basemargin, d.sidemagin + tick)
                    ctx.lineTo((i==0 || i==5 || i==10) ? 2 * width / 3: 3 * width / 4 - d.basemargin / 2, d.sidemagin + tick)
                }
            }
            if (direction == Direction.Left || direction == Direction.Right) {
                for (var i=0; i<=10; i++) {
                    var tick = i * (width - 2 * d.sidemagin)/ 10.

                    ctx.moveTo(d.sidemagin + tick, height - d.basemargin)
                    ctx.lineTo(d.sidemagin + tick, (i==0 || i==5 || i==10) ? height / 2: 3 * height / 4. - d.basemargin / 2)
                }
            }
            ctx.stroke()

            // min max value label

            if (direction == Direction.Up || direction == Direction.Down) {
                ctx.textBaseline = 'middle'
                ctx.textAlign = 'right'
                ctx.fillText(direction == Direction.Down ? t1 : t2, 2 * width / 3, d.sidemagin)
                ctx.fillText(direction == Direction.Down ? t2 : t1, 2 * width / 3, height - d.sidemagin)
            }

            if (direction == Direction.Left || direction == Direction.Right) {
                ctx.textBaseline = 'bottom'

                ctx.textAlign =  d.sidemagin > w1 / 2 ? 'center' : 'left'
                ctx.fillText(direction == Direction.Right ? t1 : t2, d.sidemagin, height / 2)

                ctx.textAlign =  d.sidemagin > w2 / 2 ? 'center' : 'right'
                ctx.fillText(direction == Direction.Right ? t2 : t1, width - d.sidemagin, height / 2)
            }


            ctx.restore()
        }
    }
}
