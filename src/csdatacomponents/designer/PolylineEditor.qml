import QtQuick 2.0

Canvas {
    id: canvas
    
    property var points: []
    property variant backendValue

    property bool closePath: false

    QtObject {
        id: d
        property size oldSize
        property var pointMoving: null
    }

    onBackendValueChanged: {
        points = eval(backendValue.expression)
        if (!points)
            points = []
        canvas.requestPaint()
    }

    onVisibleChanged: canvas.requestPaint()

    onCanvasSizeChanged: {
        if (canvasSize.width == 0 || canvasSize.height == 0)
            return
        if (d.oldSize === undefined) {
            d.oldSize = canvasSize
            return
        }
        // relocate each point within the new rectangle
        for (var i=0; i<points.length; i++) {
            points[i].x = points[i].x * canvasSize.width / d.oldSize.width
            points[i].y = points[i].y * canvasSize.height / d.oldSize.height
        }
        d.oldSize = canvasSize
        updateToBackend()
    }

    property bool __drawing: false
    onPaint: {
        console.log('paint', points)
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
        if (d.pointMoving)
            ctx.lineTo(d.pointMoving.x, d.pointMoving.y)
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
            d.pointMoving = null
            points.push(Qt.point((mouse.x), (mouse.y)))
            updateToBackend()
            parent.requestPaint()
        }
        onPositionChanged: {
            if (!__drawing)
                return
            d.pointMoving = Qt.point(mouse.x, mouse.y)
            parent.requestPaint()
        }

        onDoubleClicked: {
            __drawing = false
            updateToBackend()
        }
    }
    function updateToBackend() {
        var expr = '['
        for (var i=0; i<points.length; i++) {
            expr += 'Qt.point(%1,%2)'.arg(points[i].x).arg(points[i].y)
            if (i < points.length - 1)
               expr += ','
        }
        expr += ']'
        backendValue.expression = expr
    }
}
