import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    property alias source: animation.source
    property string imageCalc: ''

    Connections {
        target: dynamicAttr
        onUpdate: {
            if (imageCalc != '') {
                var frame = Utils.calculate(imageCalc, dynamicAttr._arg)
                animation.currentFrame = frame % animation.frameCount
            }
        }
    }

    AnimatedImage {
        id: animation;
        anchors.fill: parent
        fillMode: Image.Stretch
        // play animation unless calculation is used
        playing: imageCalc == ''
    }
}

