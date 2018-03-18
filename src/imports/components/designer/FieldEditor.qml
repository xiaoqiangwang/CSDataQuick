import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

TextField {
    id: lineEdit

    property color borderColor: "#222"
    property color highlightColor: "orange"
    property color textColor: "white"

    style: TextFieldStyle {
        selectionColor: lineEdit.textColor
        selectedTextColor: "black"
        textColor: lineEdit.textColor
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 23
            border.color: borderColor
            gradient: Gradient {
                GradientStop {color: "#2c2c2c" ; position: 0}
                GradientStop {color: "#343434" ; position: 0.15}
                GradientStop {color: "#373737" ; position: 1.0}
            }
            Rectangle {
                border.color: lineEdit.highlightColor
                anchors.fill: parent
                anchors.margins: -1
                color: "transparent"
                radius: 4
                opacity: 0.3
                visible: control.activeFocus
            }
        }
    }
}
