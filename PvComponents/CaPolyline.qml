import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    property int lineWidth: 1
    property var points: []

    onPointsChanged: canvas.requestPaint()
    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            if (points.length == 0)
                return
            var ctx = getContext('2d')
            ctx.save()
            ctx.beginPath()

            ctx.moveTo(points[0].x, points[0].y)
            for(var i=1; i<points.length; i++) {
                ctx.lineTo(points[i].x, points[i].y)
            }
            ctx.lineWidth = lineWidth
            ctx.stroke()
            ctx.restore()
        }
    }
}
