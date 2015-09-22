import QtQuick 2.0

import PvComponents 1.0

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

    onMinimumValueChanged: canvas.requestPaint()
    onMaximumValueChanged: canvas.requestPaint()
    onForegroundChanged: canvas.requestPaint()
    onDirectionChanged: canvas.requestPaint()
    onVisibleChanged: canvas.requestPaint()

    Text {
        id: hiddenText
        anchors.fill: parent
        z: -1
        text: {
            var t1 = minimumValue.toFixed(precision);
            var t2 = maximumValue.toFixed(precision);
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
            ctx.font = fontString;
            var t1 = minimumValue.toFixed(precision)
            var t2 = maximumValue.toFixed(precision)

            var w1 = ctx.measureText(t1).width
            var w2 = ctx.measureText(t2).width

            // baseline
            var baselinePos
            var lineWidth = 1
            ctx.lineWidth = lineWidth
            ctx.strokeStyle = foreground
            ctx.beginPath()
            if (orientation == Qt.Vertical) {
                baselinePos = (tickmarkAligment == Qt.AlignLeft ? width - d.basemargin : d.basemargin)
                ctx.moveTo(baselinePos, sidemargin)
                ctx.lineTo(baselinePos, height - sidemargin)
                ctx.stroke()
            }

            if (orientation == Qt.Horizontal) {
                baselinePos = (tickmarkAligment == Qt.AlignBottom ? d.basemargin : height - d.basemargin)
                ctx.moveTo(sidemargin, baselinePos)
                ctx.lineTo(width - sidemargin, baselinePos)
                ctx.stroke()
            }

            // ticks
            var tickmarkPos
            ctx.beginPath()
            if (orientation == Qt.Vertical) {
                for (var i=0; i<=10; i++) {
                    var tickPos = i * (height - 2 * sidemargin)/ 10.
                    var sign = (tickmarkAligment == Qt.AlignLeft ? -1 : 1)

                    ctx.moveTo(baselinePos, sidemargin + tickPos)
                    ctx.lineTo((i==0 || i==5 || i==10) ? (baselinePos + sign * tickLength) : (baselinePos + sign * 2*tickLength/3),
                                                         sidemargin + tickPos)
                }
            }
            if (orientation == Qt.Horizontal) {
                for (var i=0; i<=10; i++) {
                    var tickPos = i * (width - 2 * sidemargin)/ 10.
                    var sign = (tickmarkAligment == Qt.AlignBottom ? 1 : -1)

                    ctx.moveTo(sidemargin + tickPos, baselinePos)
                    ctx.lineTo(sidemargin + tickPos,
                               (i==0 || i==5 || i==10) ? (baselinePos + sign * tickLength) : (baselinePos + sign * 2*tickLength/3))
                }
            }
            ctx.stroke()

            // min max value label
            ctx.font = fontString;
            ctx.fillStyle = foreground

            var labelPos = baselinePos + sign * tickLength
            if (orientation == Qt.Vertical) {
                ctx.textBaseline = 'middle'
                ctx.textAlign = (tickmarkAligment == Qt.AlignLeft ? 'right' : 'left')
                ctx.fillText(direction == Direction.Down ? t1 : t2, labelPos, sidemargin)
                ctx.fillText(direction == Direction.Down ? t2 : t1, labelPos, height - sidemargin)
            }

            if (orientation == Qt.Horizontal) {
                ctx.textBaseline =  (tickmarkAligment == Qt.AlignBottom ? 'top' : 'bottom')

                ctx.textAlign =  sidemargin > w1 / 2 ? 'center' : 'left'
                ctx.fillText(direction == Direction.Right ? t1 : t2,  0, labelPos)

                ctx.textAlign =  sidemargin > w2 / 2 ? 'center' : 'right'
                ctx.fillText(direction == Direction.Right ? t2 : t1, width, labelPos)
            }


            ctx.restore()
        }
    }
}
