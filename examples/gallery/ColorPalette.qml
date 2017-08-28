import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.2

import CSDataQuick.Components 1.0

Rectangle {
    id: colorPalette
    property int currentIndex: 1

    onCurrentIndexChanged: {
        var newColor = ColorMap['color%1'.arg(currentIndex)]
        if (newColor != color)
            colorPalette.color = newColor
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (toolTip.opacity == 0)
                show()
            else
                hide()
        }
    }

    Item {
        id: toolTip
        opacity: 0
        x: -40
        y: 20
        property Item originalParent: colorPalette

        property int oldX: x
        property int oldY: y
        
        Grid {
            columns: 13
            spacing: 1
            flow: Grid.TopToBottom

            ExclusiveGroup {
                id: colorButtonGroup
            }
            Repeater {
                model: 65
                delegate: Button {
                    width: 15
                    height: 15
                    style: ButtonStyle{
                        background: Rectangle {
                            color: ColorMap['color%1'.arg(index)]
                            border.width: checked ? 1 : 0
                        }
                    }
                    checkable: true
                    exclusiveGroup: colorButtonGroup
                    onClicked: {
                        colorPalette.color = color
                        hide()
                    }
                    enabled: toolTip.opacity != 0
                    property color color: ColorMap['color%1'.arg(index)]
                }
            }
        }
    }

    function show() {
        toolTip.originalParent = toolTip.parent;
        var p = toolTip.parent;
        while (p.parent != undefined && p.parent.parent != undefined)
            p = p.parent
        toolTip.parent = p;

        toolTip.oldX = toolTip.x
        toolTip.oldY = toolTip.y
        var globalPos = toolTip.mapFromItem(parent, toolTip.oldX, toolTip.oldY);
        toolTip.x = globalPos.x + toolTip.oldX
        toolTip.y = globalPos.y + toolTip.oldY

        toolTip.opacity = 1
    }
    function hide() {
        toolTip.opacity = 0;
        var oldClip = toolTip.originalParent.clip
        toolTip.originalParent.clip = false
        toolTip.parent = toolTip.originalParent
        toolTip.originalParent.clip = true
        toolTip.originalParent.clip = oldClip
        toolTip.x = toolTip.oldX
        toolTip.y = toolTip.oldY
    }
}
