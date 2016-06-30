import QtQuick 2.0
import QtQuick.Controls 2.0

import CSDataQuick.Components 1.0

Button {
    id: root
    property color backgroundColor: __pallete.button
    property color foregroundColor: __pallete.buttonText
    property int align: Text.AlignHCenter

    SystemPalette {
        id: __pallete
        colorGroup: SystemPalette.Active
    }

    contentItem: Text {
        text: root.text
        font: root.font
        color: root.foregroundColor
        clip: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: root.align
    }
    background: StyledFrame {
        implicitWidth: 100
        implicitHeight: 20
        color: root.backgroundColor
        shadow: root.down || root.checked ? FrameShadow.Sunken : FrameShadow.Raise
    }
}
