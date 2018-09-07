import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import CSDataQuick.Components 1.0

Button {
    id: root
    property color backgroundColor: Pallete.button
    property color foregroundColor: Pallete.buttonText
    property alias font: hiddenText.font
    property int fontSizeMode: Text.FixedSize
    property int align: Text.AlignHCenter
    property Component icon: null

    Text {
        id: hiddenText
    }

    style: ButtonStyle {
        background: StyledFrame {
            color: control.backgroundColor
            shadow: (control.pressed || control.checked) ? FrameShadow.Sunken : FrameShadow.Raise
        }

        label: Item {
            Loader {
                id: icon
                sourceComponent: root.icon
                anchors.verticalCenter: parent.verticalCenter
                width: sourceComponent !== null ? Math.min(control.height, control.width) - 6 : 0
                height: width
            }
            Text {
                id: text
                text: control.text
                anchors.left: icon.right
                anchors.right: parent.right
                height: parent.height
                clip: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: root.align
                color: control.foregroundColor
                // bind font properties
                font.bold: root.font.bold
                font.capitalization: root.font.capitalization
                font.family: root.font.family
                // font.hintingPreference: root.font.hintingPreference // Qt 5.9+ only
                font.italic: root.font.italic
                font.letterSpacing: root.font.letterSpacing
                font.pixelSize: root.font.pixelSize
                font.strikeout: root.font.strikeout
                font.styleName: root.font.styleName
                font.underline: root.font.underline
                font.weight: root.font.weight
                font.wordSpacing: root.font.wordSpacing
                minimumPixelSize: 8
                fontSizeMode: root.fontSizeMode
                Binding on font.pixelSize {
                    when: fontSizeMode != Text.FixedSize
                    value: height
                }
            }
        }
    }
}
