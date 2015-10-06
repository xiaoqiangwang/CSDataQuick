import QtQuick 2.0

import CSDataQuick.Components 1.0

Item {
    width: 700
    height: 700
    CaADImage {
        x: 10
        y: 10
        width: 600
        height: 600
        interval: 200
        source: '13SIM1:image1:'
    }
}
