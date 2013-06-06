import QtQuick 2.0
import QtQuick.Controls 1.0

import PvLibrary 1.0

Rectangle {
    PvObject {
        id: pv
    }

    property alias channel: pv.channel

    ExclusiveGroup { id: radioInputGroup }
    Row {
        id: btnRow;
    }

    Connections {
        target: pv
        onConnectionChanged: {

        }
    }
}
