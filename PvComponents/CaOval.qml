import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    onForegroundChanged: canvas.requestPaint()

    Canvas {
        id: canvas

        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.save()
            ctx.beginPath()

            ctx.ellipse(lineWidth, lineWidth, width - 2 * lineWidth, height - 2 * lineWidth)
            if (fill == FillStyle.Solid) {
                ctx.fillStyle = foreground
                ctx.fill()
            }
            ctx.lineWidth = lineWidth
            ctx.strokeStyle = foreground
            ctx.stroke()
            ctx.restore()
        }
    }
}
