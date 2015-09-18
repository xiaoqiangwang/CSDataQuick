import QtQuick 2.0
import QtQuick.Layouts 1.0

import PvComponents 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CaMeter
    \inqmlmodule PvComponents
    \brief Displays the value on a cirular dial panel.

    The value of the process variable is displayed by a needle within an arc.
*/

CaMonitor {
    id: root

    /*!
        \qmlproperty enumeration label
        The decoration mode.
    */
    property int label: LabelStyle.Frame

    /*! The operation limits */
    property Limits limits : Limits{}

    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height / 8, 0)

    Meter {
        id: meter
        anchors.fill: parent

        background: root.background
        indicatorColor: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground

        minimumValue: limits.lopr
        maximumValue: limits.hopr
        precision: limits.prec

        font.family: root.font.family
        font.pixelSize: root.font.size

        title: channel
        showTitle: label == LabelStyle.Channel
        showRange: true
        showValue: label == LabelStyle.Limits || label == LabelStyle.Channel
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
