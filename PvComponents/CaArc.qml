import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    property int begin
    property int end

    onForegroundChanged: canvas.requestPaint()
    onBeginChanged: canvas.requestPaint()
    onEndChanged: canvas.requestPaint()

    Canvas {
        id: canvas

        anchors.fill: parent
        onPaint: {
            var centery = height /2
            var centerx = width /2
            var radius = Math.min(width/2, height) * 0.85

            var ctx = getContext('2d')
            ctx.clearRect(0, 0, width, height)

            ctx.save()
            ctx.beginPath()

            ctx.translate(centerx, centery)
            ctx.arc(0, 0, radius, -begin / 180 * Math.PI, -end / 180 * Math.PI, true)

            if (fill == FillStyle.Solid) {
                ctx.endPath()
                ctx.fillStyle = foreground
                ctx.fill()
            }
            ctx.lineWidth = lineWidth
            ctx.stroke()
            ctx.restore()
        }
    }
}
