import QtQuick 2.0
import PvComponents 1.0


Item {
    width: 700
    height: 700
    CaADImage {
        x: 10
        y: 10
        width: 600
        height: 600
        interval: 200
        channel: '13SIM1:image1:'
    }
}
