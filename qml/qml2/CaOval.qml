import QtQuick 2.0

Canvas {
    property bool alarmColor: false

    onPaint: {
        var ctx = getContext('2d')
        ctx.save()
        ctx.beginPath();
        ctx.ellipse(0, 0, width, height)
        ctx.stroke();
    }
}
