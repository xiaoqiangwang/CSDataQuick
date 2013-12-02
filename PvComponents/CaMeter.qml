import QtQuick 2.0

import PvComponents 1.0

CaMonitor {
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property real value: 0.0

    // limits
    property int precision: 2

    Connections {
        target: pv
        onValueChanged: {
            value  = Number(pv.value).toFixed(precision)
            canvas.requestPaint()
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext('2d')
            ctx.clearRect(0, 0, width, height)

            var centery = height * 0.95
            var centerx = width /2
            var radius = Math.min(width/2, height) * 0.85
            var lineWidth = 1

            // dial panel
            ctx.save()
            ctx.beginPath()
            ctx.translate(centerx, centery)
            ctx.arc(0, 0, radius, 0, -Math.PI, true)

            for (var i=0; i<=10; i++) {
                ctx.save()
                ctx.rotate(i * Math.PI / 10.)
                ctx.moveTo(-radius, 0)
                ctx.lineTo(-radius * 0.8, 0)
                ctx.restore()
            }
            ctx.lineWidth = lineWidth
            ctx.stroke()
            ctx.restore()

            // needle
            ctx.beginPath()
            ctx.save()
            ctx.translate(centerx, centery)
            ctx.rotate(Math.PI * (value - minimumValue) / (maximumValue - minimumValue))
            ctx.moveTo(-radius, 0)
            ctx.lineTo(0, 0)
            ctx.lineWidth = lineWidth * 3
            ctx.stroke()
            ctx.restore()

            // text
            ctx.textAlign = 'center'
            ctx.fillText(value, centerx, centery * 0.9)
        }
    }
}
