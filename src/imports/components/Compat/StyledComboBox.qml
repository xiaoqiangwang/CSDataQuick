import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0 as Private

ComboBox {
    id: root

    property color backgroundColor: 'white'
    property color foregroundColor: 'black'
    property string displayText: currentText
    property alias font: hiddenText.font
    property int fontSizeMode: Text.FixedSize

    Text {
        id: hiddenText
    }

    style: ComboBoxStyle {
        id: cbStyle

        padding { top: 2 ; left: 2 ; right: 14 ; bottom:2 }
        
        label: Item {
            implicitWidth: textitem.implicitWidth
            baselineOffset: textitem.y + textitem.baselineOffset
            Text {
                id: textitem
                text: root.displayText
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideRight
                color: root.foregroundColor
                font: root.font
            }
        }

        background: Private.StyledFrame {
            implicitWidth: Math.round(Private.TextSingleton.implicitHeight * 4.5) + 10
            implicitHeight: Math.max(25, Math.round(Private.TextSingleton.implicitHeight * 1.2)) 
            Private.StyledFrame {
                width: 9
                height: 7
                shadow: FrameShadow.Raise
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 7
                opacity: root.enabled ? 0.7 : 0.5
                color: root.backgroundColor
            }
            shadow: FrameShadow.Raise
            color: root.backgroundColor
        }

        panel: Item {
            property bool popup: false
            property font font: cbStyle.font
            property color textColor: cbStyle.textColor
            property color selectionColor: cbStyle.selectionColor
            property color selectedTextColor: cbStyle.selectedTextColor
            property int dropDownButtonWidth: cbStyle.dropDownButtonWidth
            anchors.centerIn: parent
            anchors.fill: parent
            implicitWidth: labelLoader.implicitWidth + 20
            implicitHeight: Math.max(labelLoader.implicitHeight + padding.top + padding.bottom, backgroundLoader.implicitHeight)
            baselineOffset: labelLoader.item ? padding.top + labelLoader.item.baselineOffset: 0
            Loader {
                id: backgroundLoader
                anchors.fill: parent
                sourceComponent: background
            }

            Loader {
                id: labelLoader
                sourceComponent: label
                anchors.fill: parent
                anchors.leftMargin: padding.left
                anchors.topMargin: padding.top
                anchors.rightMargin: padding.right
                anchors.bottomMargin: padding.bottom
            }
        }
    }
}
