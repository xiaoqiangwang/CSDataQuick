import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.2

import PvComponents 1.0

Rectangle {
    property int currentIndex: 1
    color: colorButtonGroup.current.color

    MouseArea {
        anchors.fill: parent
        onClicked: {
            palette.visible = true 
        }
    }

    Dialog {
        id: palette
        width: 300
        height: 170
        Grid {
            columns: 13
            anchors.fill: parent
            spacing: 2
            flow: Grid.TopToBottom

            ExclusiveGroup {
                id: colorButtonGroup
            }
            
            Repeater {
                model: 65
                delegate: Button {
                    width: 20
                    height:20
                    style: ButtonStyle{
                        background: Rectangle{
                            color: ColorMap['color%1'.arg(index)]
                            border.width: checked ? 1 : 0
                        }
                    }
                    checkable: true
                    checked: index == currentIndex
                    exclusiveGroup: colorButtonGroup
                    property color color: ColorMap['color%1'.arg(index)]
                }
            }
        }
    }
}
