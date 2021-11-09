import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1

TextArea {
    id: root

    property color foregroundColor: 'black'
    property color backgroundColor: 'white'

    style: TextAreaStyle {
        textColor: root.foregroundColor
        backgroundColor: root.backgroundColor
    }
}
