import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents.PvObject 1.0


TextField {
    property alias channel: pv.channel

    text: pv.value

    PvObject {
        id: pv
        asstring: true
    }

    onAccepted: {
        pv.value = text
    }
}
