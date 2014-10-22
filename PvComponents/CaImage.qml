import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaImage
    \inqmlmodule PvComponents
    \brief Show a static or dynamic image
*/

CaGraphics {
    /*!
      \qmlproperty url source

       This property holds the URL that refers to the source image.
    */
    property alias source: animation.source

    /*!
        For multiple-frame images, the frame to display is defined by the expression.
    */
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
