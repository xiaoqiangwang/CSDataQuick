import QtQuick 2.1
import QtQuick.Controls 1.0
import PvComponents 1.0

Rectangle {
    Column {
        Row {
            CaTextEntry {
                channel: '13SIM1:cam1:AcquireTime'
            }
            CaMenu {
                channel: '13SIM1:cam1:Acquire'
            }
        }
        CaADImage {
            width: 200
            height: 200
            interval: 100
            channel: '13SIM1:image1:'
        }
        CaADImage {
            width: 200
            height: 200
            interval: 800
            channel: '13SIM1:image1:'
        }
    }
}
