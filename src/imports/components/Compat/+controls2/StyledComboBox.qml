import QtQml 2.1
import QtQuick 2.0
import QtQuick.Templates 2.5 as T
import QtQuick.Controls 2.5


import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0 as Private

T.ComboBox {
    id: root

    property color backgroundColor: 'white'
    property color foregroundColor: 'black'

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)



    delegate: ItemDelegate {
        text: modelData
    }

    indicator: Private.StyledFrame {
        width: 9
        height: 7
        shadow: FrameShadow.Raise
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 7
        opacity: root.enabled ? 0.7 : 0.5
        color: root.backgroundColor
    }

    contentItem: Text {
        text: root.currentText
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: root.foregroundColor
        font: root.font
    }

    background: Private.StyledFrame {
        implicitWidth: Math.round(root.implicitContentHeight * 4.5) + 10
        implicitHeight: Math.max(25, Math.round(root.implicitContentHeight * 1.2)) 
        anchors.fill: parent
        shadow: FrameShadow.Raise
        color: root.backgroundColor
    }
    popup: Menu {
        id: popupMenu
        y: root.height - 1
        Instantiator {
            id: executeMenuInst
            model: root.model
            delegate: MenuItem {
                text: modelData
                onTriggered: root.activated(index)
            }
            onObjectAdded: popupMenu.insertItem(index, object)
            onObjectRemoved: popupMenu.removeItem(object)
        }
    }
}
