import QtQuick 2.0

Canvas {
    id: canvas

    property var points: []
    property variant backendValue

    property bool closePath: false
    property int mode: 0 // 0 - drawing, 1 - selection

    QtObject {
        id: d
        property size oldSize
        property bool drawing: false
        property var pointMoving: null
        property int selectedPoint: -1
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

        if (d.selectedPoint != -1) {
            ctx.lineWidth = 2
            ctx.beginPath()
            ctx.arc(points[d.selectedPoint].x, points[d.selectedPoint].y, 3, 0, 2*Math.PI)
            ctx.stroke()
        }
    }

    Text {
        id: tooltip
        color: 'yellow'
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: canvas.mode === 0 ? Qt.ArrowCursor : Qt.CrossCursor

        onClicked: {
            if (canvas.mode === 0) {
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
            }
            else {
                d.selectedPoint = hitTest(Qt.point(mouse.x, mouse.y))
            }
            parent.requestPaint()
        }
        onPositionChanged: {
            // display mouse position
            tooltip.x = mouse.x + 10
            tooltip.y = mouse.y - 10
            tooltip.text = mouse.x + ',' + mouse.y

            if (canvas.mode === 0) {
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
            } else {
                if (d.selectedPoint == -1)
                    return
                points[d.selectedPoint].x = mouse.x
                points[d.selectedPoint].y = mouse.y
            }
            parent.requestPaint()
        }

        onDoubleClicked: {
            if (canvas.mode === 0) {
                d.pointMoving = null
                d.drawing = false
                updateToBackend()
            } else {
                points[d.selectedPoint].x = mouse.x
                points[d.selectedPoint].y = mouse.y
                updateToBackend()
                d.selectedPoint = -1
            }
            parent.requestPaint()
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
    function hitTest(pt) {
        for(var i=0; i<points.length; i++) {
            var distance = Math.sqrt(Math.pow(pt.x - points[i].x, 2) + Math.pow(pt.y - points[i].y, 2))
            if (distance <= 3)
                return i;
        }
        return -1;
    }
}
