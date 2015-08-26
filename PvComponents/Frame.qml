import QtQuick 2.1

import PvComponents 1.0

Rectangle {
    implicitHeight: 10
    implicitWidth: 10
    property int shadow: FrameShadow.Flat

    BorderImage {
        id: border
        source: shadow == FrameShadow.Sunken ? 'images/button_down.png' : 'images/button_up.png'
        border {left: 3; right: 3; top: 3; bottom: 3;}
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        width: parent.width
        height: parent.height
    }

    border.width: shadow == FrameShadow.Flat ? 1 : 0
}
