import QtQuick 2.1

import CSDataQuick.Components 1.0

Item {
    id: root
    implicitWidth: textField.implicitWidth
    implicitHeight: textField.implicitHeight

    property color background: 'white'
    property color foreground : 'black'
    property alias readOnly: textField.readOnly
    property alias font: textField.font
    property alias horizontalAlignment: textField.horizontalAlignment
    property alias hasFocus: textField.activeFocus
    property alias cursorPosition: textField.cursorPosition
    property string text
    signal accepted

    onTextChanged: textField.text = text

    StyledFrame {
        anchors.fill: parent
        color: root.background
        shadow: FrameShadow.Sunken
    }

    TextInput {
        id: textField
        color: root.foreground
        verticalAlignment: TextInput.AlignBottom
        activeFocusOnPress: true
        clip: true
        selectByMouse: true
        anchors.topMargin: 2
        anchors.bottomMargin: 2
        anchors.leftMargin: 2
        anchors.rightMargin: 2
        anchors.fill: parent
        onAccepted: {
            root.text = text
            root.accepted()
        }
    }

    MouseArea {
        anchors.fill: textField
        acceptedButtons: Qt.NoButton
        hoverEnabled: true
        onEntered: textField.focus = true
        onExited: {
            textField.focus = false
            textField.text = root.text
        }
        DropArea {
            anchors.fill: parent
            keys: ['text/plain']
            onDropped: textField.text = drop.text
        }
    }
}
