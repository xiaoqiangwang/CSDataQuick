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
            interval: 100
            channel: '13SIM1:image1:'
        }
    }
}
