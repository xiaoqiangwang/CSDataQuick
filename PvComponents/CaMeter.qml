import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaMeter
    \inqmlmodule PvComponents
    \brief Displays a dial indicator
*/

CaMonitor {
    id: control
    /*! The operation limits */
    property Limits limits : Limits{}

    Meter {
        id: meter
        anchors.fill: parent
        foreground: control.foreground
        background: control.background
        minimumValue: limits.lopr
        maximumValue: limits.hopr
    }

    Connections {
        target: pv
        onConnectionChanged: {
            if (pv.connected) {
                if (pv.lodisplim < pv.updisplim) {
                    limits.loprChannel = pv.lodisplim
                    limits.hoprChannel = pv.updisplim
                }
                limits.precChannel = pv.prec
            }
        }
        onValueChanged: {
            meter.value = Number(pv.value).toFixed(limits.prec)
        }
    }
}
