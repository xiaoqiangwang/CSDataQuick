import QtQuick 2.1
import QtQuick.Templates 2.5 as T
import QtQuick.Controls 2.5

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0 as Private

T.Button {
    id: root
    property color backgroundColor: Pallete.button
    property color foregroundColor: Pallete.buttonText
    property int fontSizeMode: Text.FixedSize
    property int align: Text.AlignHCenter
    property Component iconItem: null
    property var exclusiveGroup: null


    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

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
                font: root.font
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
