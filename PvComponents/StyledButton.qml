import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Button {
    property color background: __pallete.button
    property color foreground: __pallete.buttonText
    property int pixelSize: 12
    property string  fontFamily//: fontSize <= 22 ? 'Dejavu Sans Mono' : 'Helvetica'

    SystemPalette {
        id: __pallete
        colorGroup: SystemPalette.Active
    }

    style: ButtonStyle {
        background: Rectangle {
            implicitHeight: 10
            implicitWidth: 10
            Image {
                id: icon
                source: control.iconSource
                anchors.leftMargin: 2
                width: Math.min(control.height, control.width) * 0.8
                height: Math.min(control.height, control.width) * 0.8
            }

            BorderImage {
                source: control.pressed || control.checked ? 'images/button_down.png' : 'images/button_up.png'
                border {left: 3; right: 3; top: 3; bottom: 3;}
                horizontalTileMode: BorderImage.Stretch
                verticalTileMode: BorderImage.Stretch
                width: control.width
                height: control.height
            }
            color: control.background
        }

        label: Text {
            text: control.text
            anchors.fill: parent
            anchors.leftMargin: control.iconSource == '' ? 0 : Math.min(control.height, control.width) * 0.8
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: control.foreground
            font.pixelSize: control.pixelSize
            font.family: control.fontFamily
        }
    }
}
