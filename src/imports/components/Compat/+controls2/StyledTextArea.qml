import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Templates 2.5 as T

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0 as Private

T.TextArea {
    id: root

    property color foregroundColor: 'black'
    property color backgroundColor: 'white'

    implicitWidth: Math.max(contentWidth,
                            implicitBackgroundWidth + leftInset + rightInset)
    implicitHeight: Math.max(contentHeight,
                             implicitBackgroundHeight + topInset + bottomInset)

    color: root.foregroundColor

    background: Private.StyledFrame {
        color: root.backgroundColor
        shadow: FrameShadow.Sunken
    }
}
