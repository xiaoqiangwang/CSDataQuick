import QtQuick 2.0

Canvas {
    id: canvas
    
    property var points: []
    property variant backendValue

    property bool closePath: false

    QtObject {
        id: d
        property size oldSize
        property bool drawing: false
        property var pointMoving: null
    }

    Timer {
        id: timer
        interval: 500;
        onTriggered: {
            updateToBackend()
        }
    }

    Connections {
        target: modelNodeBackend
        onSelectionToBeChanged: {
            timer.stop()
            updateToBackend()
        }
        onModelNodeChanged: {
            console.log('model changed')
            updateFromBackend()
        }
    }

    onBackendValueChanged: {
        // stop active timer in case value changed outside of editor
        timer.stop()
        updateFromBackend()
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
        timer.restart()
    }

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
            if (!d.drawing) {
                points.length = 0
                d.drawing = true
                parent.requestPaint()
            }
            if (d.pointMoving) {
                points.push(Qt.point(d.pointMoving.x, d.pointMoving.y))
                d.pointMoving = null
            }
            else
                points.push(Qt.point(mouse.x, mouse.y))
            updateToBackend()
            parent.requestPaint()
        }
        onPositionChanged: {
            if (!d.drawing)
                return
            if (mouse.modifiers & Qt.ShiftModifier) {
                var pt = points[points.length-1]
                if (Math.abs(mouse.y - pt.y) > Math.abs(mouse.x - pt.x))
                    d.pointMoving = Qt.point(pt.x, mouse.y)
                else
                    d.pointMoving = Qt.point(mouse.x, pt.y)
            } else {
                d.pointMoving = Qt.point(mouse.x, mouse.y)
            }
            parent.requestPaint()
        }

        onDoubleClicked: {
            d.pointMoving = null
            d.drawing = false
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
    function updateFromBackend() {
        points = eval(backendValue.expression)
        if (!points)
            points = []
        canvas.requestPaint()
    }
}
