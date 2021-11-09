import QtQuick 2.5

import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

SpinBox {
    id: root

    property alias doubleValue: root.value
    property alias suffixText: root.suffix
    property color backgroundColor: 'white'
    property color foregroundColor: 'black'
    stepSize: Math.pow(10, -decimals)

    style: SpinBoxStyle {
        font: root.font
        textColor: root.foregroundColor
        selectedTextColor: Qt.lighter(textColor)
        selectionColor: Qt.darker(root.backgroundColor)
        background: Rectangle {
            implicitHeight: Math.max(25, Math.round(styleData.contentHeight * 1.2))
            implicitWidth: styleData.contentWidth + padding.left + padding.right
            baselineOffset: root.__baselineOffset
            color: root.backgroundColor
        }
    }
}

