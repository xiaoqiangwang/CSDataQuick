import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

CSControl {
    id: root
    implicitWidth: 200
    implicitHeight: 60

    property alias font: textArea.font

    TextArea {
        id: textArea
        anchors.fill: parent
        style: TextAreaStyle {
            textColor: root.foreground
            backgroundColor: root.background
        }
    }

    Connections {
        target: pv
        onValueChanged: {
           textArea.text = formatString()
        }
    }
    MouseArea {
        anchors.fill: root
        acceptedButtons: Qt.NoButton
        hoverEnabled: true
        onEntered: textArea.focus = true
        onExited: {
            textArea.focus = false
            textArea.text = formatString()
        }
    }

    QtObject {
        id: d
        property string text
    }
    Connections {
        target: contextMenu
        onAboutToShow: {
            d.text = textArea.text
        }
    }

    Component.onCompleted: {
        var item = contextMenu.addItem("Apply")
        item.triggered.connect(apply)
    }

    function apply() {
        pv.value = d.text
    }

    function formatString() {
        if (pv.fieldType == CSData.String)
            return pv.value
        else if (pv.fieldType == CSData.Char)
            return String.fromCharCode.apply(null, pv.value).replace(/\0/g, '')
        else
            return ''
    }
}
