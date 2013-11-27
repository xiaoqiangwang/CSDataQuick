import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Button {
    property color background: __pallete.button
    property color foreground: __pallete.buttonText

    SystemPalette {
        id: __pallete
        colorGroup: SystemPalette.Active
    }

    style: ButtonStyle {
        background: Rectangle {
            BorderImage {
                id: name
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
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: control.foreground
        }
    }
}
