import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

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
            Image {
                id: icon
                source: control.iconSource
                anchors.leftMargin: - 4
                anchors.verticalCenter: parent.verticalCenter
                width: Math.min(control.height, control.width) - 6
                height: width
            }

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

        label: Text {
            id: text
            text: control.text
            anchors.fill: parent
            clip: true
            anchors.leftMargin: control.iconSource == '' ? 0 : Math.min(control.height, control.width) - 8
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: root.align
            color: control.foreground
            font.pixelSize: control.pixelSize
            font.family: control.fontFamily
        }
    }
}
