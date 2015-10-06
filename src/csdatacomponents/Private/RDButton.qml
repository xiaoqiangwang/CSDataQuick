import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import CSDataQuick.Components 1.0

Button {
    id: root
    property color background: __pallete.button
    property color foreground: __pallete.buttonText
    property int pixelSize: 12
    property string  fontFamily
    property int align: Text.AlignHCenter

    SystemPalette {
        id: __pallete
        colorGroup: SystemPalette.Active
    }

    style: ButtonStyle {
        background: Rectangle {
            implicitHeight: 10
            implicitWidth: 10

            BorderImage {
                id: border
                source: control.pressed || control.checked ? 'images/button_down.png' : 'images/button_up.png'
                border {left: 2; right: 2; top: 2; bottom: 2;}
                horizontalTileMode: BorderImage.Stretch
                verticalTileMode: BorderImage.Stretch
                smooth: false
                width: control.width
                height: control.height
            }
            color: control.background
        }

        label: Item {
            implicitWidth: row.implicitWidth
            implicitHeight: row.implicitHeight

            Row {
                id: row
                anchors.fill: parent
                DoubleRect {
                    id: rect
                    width: Math.min(control.height, control.width) - 6
                    height: width
                    anchors.verticalCenter: parent.verticalCenter
                    foreground: root.foreground
                }

                Text {
                    id: text
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter

                    text: control.text
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: root.align
                    color: control.foreground
                    font.pixelSize: control.pixelSize
                    font.family: control.fontFamily
                }
            }
        }
    }
}
