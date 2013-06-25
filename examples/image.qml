import QtQuick 2.1
import QtQuick.Controls 1.0
import PvComponents 1.0

Rectangle {
    Column{
    Row{
    CaTextEntry {
        channel: '13SIM1:cam1:AcquireTime'
    }
    CaMenu {
        channel: '13SIM1:cam1:Acquire'
    }
    }
    Timer {
        id: timer
        interval: 100
        running: true
        repeat: true
        onTriggered: {
            img.source = ''
            img.source = 'image://areaDetector/13SIM1:image1:'
        }
    }

    Image { 
        id: img
        source: 'image://areaDetector/13SIM1:image1:'
    }
    }
}
