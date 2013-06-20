import QtQuick 2.0
import QtQuick.Controls 1.0

import PvLibrary 1.0


Button {
    property alias channel: pv.channel
    property var onMessage
    property var offMessage

    PvObject {
        id: pv
    }

    onPressedChanged: {
        if (pressed)
            pv.value = onMessage
        else
            pv.value = offMessage
    }
}
