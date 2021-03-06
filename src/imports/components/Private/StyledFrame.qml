import QtQuick 2.1

import CSDataQuick.Components 1.0

Rectangle {
    id: root
    implicitHeight: 10
    implicitWidth: 10
    property int shadow: FrameShadow.Flat
    property int innerMargin: shadow == FrameShadow.Flat ? 0 : 2
    property int contentWidth: shadow == FrameShadow.Flat ? width : width - 4
    property int contentHeight: shadow == FrameShadow.Flat ? height : height - 4

    BorderImage {
        id: border
        source: root.shadow == FrameShadow.Sunken ? 'images/button_down.png' : (root.shadow == FrameShadow.Raise ? 'images/button_up.png' : '')
        border {left: 2; right: 2; top: 2; bottom: 2;}
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        smooth: false
        width: parent.width
        height: parent.height
    }
}
