import QtQuick 2.1
import QtQuick.Controls 2.2

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0 as Private

Button {
    id: root
    property color backgroundColor: Pallete.button
    property color foregroundColor: Pallete.buttonText
    property int fontSizeMode: Text.FixedSize
    property int align: Text.AlignHCenter
    property Component iconItem: null
    property var exclusiveGroup: null

    padding: 3

    background: Private.StyledFrame {
        implicitWidth: Math.round(Private.TextSingleton.implicitHeight * 4.5)
        implicitHeight: Math.max(25, Math.round(Private.TextSingleton.implicitHeight * 1.2))
        color: root.backgroundColor
        shadow: (root.pressed || root.checked) ? FrameShadow.Sunken : FrameShadow.Raise
    }

    contentItem: Item {
        clip: true
        Row {
            height: parent.height
            anchors.left: root.align & Text.AlignLeft ? parent.Left : undefined
            anchors.right: root.align & Text.AlignRight ? parent.right : undefined
            anchors.horizontalCenter: root.align & Text.AlignHCenter ? parent.horizontalCenter : undefined
            anchors.verticalCenter: parent.verticalCenter
            spacing: 1
            Loader {
                sourceComponent: root.iconItem
                anchors.verticalCenter: parent.verticalCenter
                width: sourceComponent !== null ? Math.min(root.height, root.width) - 6 : 0
                height: width
            }
            Text {
                text: root.text
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: root.align
                color: root.foregroundColor
                // bind font properties
                font.bold: root.font.bold
                font.capitalization: root.font.capitalization
                font.family: root.font.family
                font.hintingPreference: root.font.hintingPreference
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
                    value: height
                }
            }
        }
    }

    ButtonGroup.group: root.exclusiveGroup
}
