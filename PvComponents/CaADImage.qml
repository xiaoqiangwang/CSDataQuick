import QtQuick 2.1
import QtQuick.Controls 1.0
import PvComponents 1.0

Image {
    property alias interval: timer.interval
    property string channel

    Timer {
        id: timer
        running: true
        repeat: true
        onTriggered: {
            source = ''
            source = 'image://areaDetector/'+ channel
        }
    }
}
