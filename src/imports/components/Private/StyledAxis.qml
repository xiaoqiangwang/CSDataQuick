import QtQml 2.0
import QtQuick 2.0

import CSDataQuick.Components 1.0

Rectangle {
    id: root

    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property int precision: 0
    property alias font: hiddenText.font
    property int direction: Direction.Right
    property color foreground
    property color background
    property int tickmarkAligment: orientation == Qt.Horizontal ? Qt.AlignTop : Qt.AlignLeft

    readonly property int orientation: (direction == Direction.Left || direction == Direction.Right) ? Qt.Horizontal : Qt.Vertical
    readonly property string fontString: '%1px "%2"'.arg(font.pixelSize).arg(font.family)
    readonly property int sidemargin: Math.min(font.pixelSize, 20)
    readonly property int tickLength: orientation == Qt.Horizontal
                                      ? Math.min((height - font.pixelSize), height / 3)
                                      : Math.min(width - hiddenText.paintedWidth, width / 3)
    implicitWidth: orientation == Qt.Horizontal ?  100 : hiddenText.paintedWidth + font.pixelSize
    implicitHeight: orientation == Qt.Horizontal ? font.pixelSize * 2: 100

    color: background

    onPrecisionChanged: canvas.requestPaint()
    onMinimumValueChanged: canvas.requestPaint()
    onMaximumValueChanged: canvas.requestPaint()
    onForegroundChanged: canvas.requestPaint()
    onDirectionChanged: canvas.requestPaint()
    onVisibleChanged: canvas.requestPaint()
    onTickmarkAligmentChanged: canvas.requestPaint()

    Text {
        id: hiddenText
        anchors.fill: parent
        z: -1
        text: {
            var t1 = root.minimumValue.toFixed(root.precision);
            var t2 = root.maximumValue.toFixed(root.precision);
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
            ctx.font = root.fontString;
            var t1 = root.minimumValue.toFixed(root.precision)
            var t2 = root.maximumValue.toFixed(root.precision)

            var w1 = ctx.measureText(t1).width
            var w2 = ctx.measureText(t2).width

            // baseline
            var baselinePos
            var lineWidth = 1
            ctx.lineWidth = lineWidth
            ctx.strokeStyle = root.foreground
            ctx.beginPath()
            if (root.orientation == Qt.Vertical) {
                baselinePos = (root.tickmarkAligment == Qt.AlignLeft ? width - d.basemargin : d.basemargin)
                ctx.moveTo(baselinePos, root.sidemargin)
                ctx.lineTo(baselinePos, canvas.height - root.sidemargin)
                ctx.stroke()
            }

            if (root.orientation == Qt.Horizontal) {
                baselinePos = (root.tickmarkAligment == Qt.AlignBottom ? d.basemargin : height - d.basemargin)
                ctx.moveTo(root.sidemargin, baselinePos)
                ctx.lineTo(canvas.width - root.sidemargin, baselinePos)
                ctx.stroke()
            }

            // ticks
            var tickmarkPos
            ctx.beginPath()
            if (root.orientation == Qt.Vertical) {
                for (var i=0; i<=10; i++) {
                    var tickPos = i * (canvas.height - 2 * root.sidemargin)/ 10.
                    var sign = (root.tickmarkAligment == Qt.AlignLeft ? -1 : 1)

                    ctx.moveTo(baselinePos, root.sidemargin + tickPos)
                    ctx.lineTo((i==0 || i==5 || i==10) ? (baselinePos + sign * root.tickLength) : (baselinePos + sign * 2*root.tickLength/3),
                                                         root.sidemargin + tickPos)
                }
            }
            if (root.orientation == Qt.Horizontal) {
                for (var i=0; i<=10; i++) {
                    var tickPos = i * (canvas.width - 2 * root.sidemargin)/ 10.
                    var sign = (root.tickmarkAligment == Qt.AlignBottom ? 1 : -1)

                    ctx.moveTo(root.sidemargin + tickPos, baselinePos)
                    ctx.lineTo(root.sidemargin + tickPos,
                               (i==0 || i==5 || i==10) ? (baselinePos + sign * root.tickLength) : (baselinePos + sign * 2*root.tickLength/3))
                }
            }
            ctx.stroke()

            // min max value label
            ctx.font = root.fontString;
            ctx.fillStyle = root.foreground

            var labelPos = baselinePos + sign * root.tickLength
            if (root.orientation == Qt.Vertical) {
                ctx.textBaseline = 'middle'
                ctx.textAlign = (root.tickmarkAligment == Qt.AlignLeft ? 'right' : 'left')
                ctx.fillText(root.direction == Direction.Down ? t1 : t2, labelPos, root.sidemargin)
                ctx.fillText(root.direction == Direction.Down ? t2 : t1, labelPos, canvas.height - root.sidemargin)
            }

            if (root.orientation == Qt.Horizontal) {
                ctx.textBaseline =  (root.tickmarkAligment == Qt.AlignBottom ? 'top' : 'bottom')

                ctx.textAlign =  root.sidemargin > w1 / 2 ? 'center' : 'left'
                ctx.fillText(root.direction == Direction.Right ? t1 : t2,  0, labelPos)

                ctx.textAlign =  root.sidemargin > w2 / 2 ? 'center' : 'right'
                ctx.fillText(root.direction == Direction.Right ? t2 : t1, width, labelPos)
            }


            ctx.restore()
        }
    }
}
