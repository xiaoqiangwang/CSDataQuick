import QtQuick 2.0
import QtQuick.Controls 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0

Item {
    id: control

    property color backgroundColor: __pallete.button
    property color foregroundColor: 'black'
    property bool checked: false
    property alias text: label.text
    property alias font: hiddenText.font
    property alias fontSizeMode: hiddenText.fontSizeMode

    implicitWidth:  hiddenText.implicitWidth
    implicitHeight: Math.max(20, Math.round(TextSingleton.implicitHeight * 1.2)) 

    onForegroundColorChanged: canvas.requestPaint()

    // text to calculate font size
    Text {
        id: hiddenText
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        text: "\u2003 " + control.text
        minimumPointSize: 5
        visible: false
    }

    // indicator
    StyledFrame {
        id: indicator
        height: hiddenText.fontInfo.pixelSize * 0.8
        width: height
        anchors.verticalCenter: label.verticalCenter
        color: control.backgroundColor
        shadow: control.checked ? FrameShadow.Sunken : FrameShadow.Raise
        Canvas {
            id: canvas
            opacity: control.checked ? 1 : 0
            anchors.fill: parent
            Behavior on opacity {NumberAnimation {duration: 80}}
            onPaint: {
                var ctx = getContext('2d')
                ctx.save()
                ctx.clearRect(0, 0, width, height)
                ctx.fillStyle = control.foregroundColor
                ctx.beginPath()
                var rx = width / 17.727
                var ry = height / 20.974
                ctx.moveTo(4.933236 * rx, 17.236857 * ry);
                ctx.bezierCurveTo(3.044956*rx, 14.631586*ry, 1.350000*rx, 12.162500*ry, 1.166667*rx, 11.750000*ry);
                ctx.bezierCurveTo(0.520454*rx, 10.296022*ry, 4.772182*rx, 11.020570*ry, 6.071203*rx, 12.585795*ry);
                ctx.bezierCurveTo(7.219158*rx, 13.968996*ry, 7.811462*rx, 13.330218*ry, 10.707437*rx, 7.585795*ry);
                ctx.bezierCurveTo(13.459096*rx, 2.127635*ry, 14.453273*rx, 1.000000*ry, 16.513789*rx, 1.000000*ry);
                ctx.bezierCurveTo(19.595716*rx, 1.000000*ry, 19.596864*rx, 1.252929*ry, 16.529959*rx, 4.562177*ry);
                ctx.bezierCurveTo(15.171435*rx, 6.028048*ry, 12.778891*rx, 10.545319*ry, 11.213195*rx, 14.600556*ry);
                ctx.lineTo(8.366473*rx, 21.973714*ry);
                ctx.fill()
                ctx.restore()
            }
        }

        /*
        Image {
            source: "images/check.svg"
            fillMode: Image.PreserveAspectFit
            opacity: control.checked ? 1 : 0
            anchors.fill: parent
            Behavior on opacity {NumberAnimation {duration: 80}}
        }
        */
    }

    // label
    Text {
        id: label
        height: hiddenText.contentsHeight
        verticalAlignment: Text.AlignVCenter
        anchors.left: indicator.right
        anchors.right: control.right
        anchors.leftMargin: TextSingleton.implicitHeight / 4
        anchors.verticalCenter: control.verticalCenter
        color: control.foregroundColor
        font.family: hiddenText.fontInfo.family
        font.pointSize: hiddenText.fontInfo.pointSize
        font.weight: hiddenText.fontInfo.weight
    }

    // (un-)check when mouse click
    MouseArea {
        anchors.fill: parent
        onClicked: control.checked = !control.checked
    }
}
