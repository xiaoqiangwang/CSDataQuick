import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.4

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0 as Private

TextArea {
    id: root

    property color foregroundColor: 'black'
    property color backgroundColor: 'white'

    backgroundVisible: false

    style: TextAreaStyle {
        textColor: root.foregroundColor

        frame: Private.StyledFrame {
            color: root.backgroundColor
            shadow: FrameShadow.Sunken
        }
    }
}
