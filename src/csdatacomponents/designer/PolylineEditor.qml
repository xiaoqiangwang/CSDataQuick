import QtQuick 2.0

Canvas {
    id: canvas
    
    property var points: []
    property variant backendValue
    property variant valueFromBackend: backendValue.expression
    property var pointMoving: null

    property bool closePath: false

    onBackendValueChanged: {
        points = backendValue.value
        canvas.requestPaint()
    }

    onValueFromBackendChanged: {
        points = eval(valueFromBackend)
        canvas.requestPaint()
    }

    property bool __drawing: false
    onPaint: {
        var ctx = getContext("2d")
        ctx.fillStyle = "black"
        ctx.fillRect(0, 0, width, height)
        ctx.strokeStyle = "red"
        ctx.lineWidth = 1
        ctx.beginPath()
        for (var i=0; i<points.length; i++) {
            if (i == 0)
                ctx.moveTo(points[i].x, points[i].y)
            else
                ctx.lineTo(points[i].x, points[i].y)
        }
        if (pointMoving)
            ctx.lineTo(pointMoving.x, pointMoving.y)
        else if (closePath)
            ctx.closePath()

        ctx.stroke()
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            // clear trace
            if (!__drawing) {
                points.length = 0
                __drawing = true
                parent.requestPaint()
            }
            pointMoving = null
            points.push(Qt.point(mouse.x, mouse.y))
            parent.requestPaint()
        }
        onPositionChanged: {
            if (!__drawing)
                return
            pointMoving = Qt.point(mouse.x, mouse.y)
            parent.requestPaint()
        }

        onDoubleClicked: {
            __drawing = false
            var expr = '['
            for (var i=0; i<points.length; i++) {
                expr += 'Qt.point(%1,%2)'.arg(points[i].x).arg(points[i].y)
                if (i < points.length - 1)
                   expr += ','
            }
            expr += ']'
            console.log(backendValue.expression)
            backendValue.expression = expr
        }
    }
}
