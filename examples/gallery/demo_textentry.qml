import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaTextEntry {
            channel: 'catest'
            width: 100
            height: 20
        }
        CaTextEntry {
            channel: 'castr'
            width: 100
            height: 20
        }
    }
}
