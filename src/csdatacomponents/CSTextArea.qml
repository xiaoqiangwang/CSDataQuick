import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

/*!
    \qmltype CSTextArea
    \inqmlmodule CSDataQuick.Components
    \brief Display an editable multiline text

    It can be used to edit a multiline text. The CSData object could be of char array type.
    To confirm the changes, use the \e Apply context menu.

    \qml
     CSTextArea {
        width: 100
        height: 80
        source: 'wavec'
    }
    \endqml

    \image textarea.png
*/

CSControl {
    id: root

    /*!
        \qmlproperty font font
        This property holds the font group property used to display the text
    */
    property alias font: textArea.font

    implicitWidth: 200
    implicitHeight: 60

    TextArea {
        id: textArea
        anchors.fill: parent
        style: TextAreaStyle {
            textColor: root.foreground
            backgroundColor: root.background
        }
    }

    Connections {
        target: csdata
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

    /*! \internal */
    function apply() {
        csdata.value = d.text
    }
    /*! \internal */
    function formatString() {
        if (csdata.fieldType == CSData.String)
            return csdata.value
        else if (csdata.fieldType == CSData.Char)
            return String.fromCharCode.apply(null, csdata.value).replace(/\0/g, '')
        else
            return ''
    }
}
