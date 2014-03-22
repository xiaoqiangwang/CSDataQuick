import QtQuick 2.1
import QtQuick.Controls 1.0
import PvComponents 1.0

Image {
    property alias interval: timer.interval
    property string channel

    cache: false
    fillMode: Image.PreserveAspectFit

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
