import QtQuick 2.0

import PvComponents 1.0

CaGraphics {
    property alias source: animation.source
    property string imageCalc: ''

    Connections {
        target: da
        onUpdate: {
            if (imageCalc != '') {
                var frame = Utils.calculate(imageCalc, da._arg)
                animation.currentFrame = frame
            }
        }
    }

    AnimatedImage {
        id: animation;
        playing: imageCalc == ''
    }
}
