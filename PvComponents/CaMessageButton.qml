import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0


CaControl {
    id: messageButton

    property alias text: btn.text

    property var onMessage
    property var offMessage

    StyledButton {
        id: btn
        background: messageButton.background
        foreground: messageButton.foreground

        onPressedChanged: {
            if (pressed)
                pv.value = onMessage
            else
                pv.value = offMessage
        }
    }
}
