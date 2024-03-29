import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0 as Private

Button {
    id: root
    property color backgroundColor: Pallete.button
    property color foregroundColor: Pallete.buttonText
    property alias font: hiddenText.font
    property int fontSizeMode: Text.FixedSize
    property int align: Text.AlignHCenter
    property Component iconItem: null

    Text {
        id: hiddenText
    }

    style: ButtonStyle {
        background: Private.StyledFrame {
            implicitWidth: Math.round(Private.TextSingleton.implicitHeight * 4.5)
            implicitHeight: Math.max(25, Math.round(Private.TextSingleton.implicitHeight * 1.2))
            color: control.backgroundColor
            shadow: (control.pressed || control.checked) ? FrameShadow.Sunken : FrameShadow.Raise
        }

        label: Item {
            clip: true
            implicitWidth: row.implicitWidth
            implicitHeight: root.fontSizeMode == Text.FixedSize ? text.implicitHeight :Private.TextSingleton.implicitHeight

            Row {
                id: row
                height: parent.height
                anchors.left: root.align & Text.AlignLeft ? parent.Left : undefined
                anchors.right: root.align & Text.AlignRight ? parent.right : undefined
                anchors.horizontalCenter: root.align & Text.AlignHCenter ? parent.horizontalCenter : undefined
                anchors.verticalCenter: parent.verticalCenter
                Loader {
                    id: icon
                    sourceComponent: root.iconItem
                    anchors.verticalCenter: parent.verticalCenter
                    width: sourceComponent !== null ? Math.min(control.height, control.width) - 6 : 0
                    height: width
                }
                Text {
                    id: text
                    text: control.text
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
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
                        when: root.fontSizeMode != Text.FixedSize
                        value: text.height
                    }
                }
            }
        }
    }
}
