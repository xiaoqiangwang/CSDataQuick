import QtQuick 2.0

import PvComponents 1.0

QtObject {
    property int  loprSrc: LimitsSource.Channel
    property real loprChannel: 0.0
    property real loprDefault: 0.0
    property real loprUser: 0.0
    property real lopr: loprSrc == LimitsSource.Channel ? loprChannel : (loprSrc == LimitsSource.Default ? loprDefault : loprUser)

    property int  hoprSrc: LimitsSource.Channel
    property real hoprChannel: 1.0
    property real hoprDefault: 1.0
    property real hoprUser: 1.0
    property real hopr: hoprSrc == LimitsSource.Channel ? hoprChannel : (hoprSrc == LimitsSource.Default ? hoprDefault : hoprUser)

    property int  precSrc: LimitsSource.Channel
    property int  precChannel: 0
    property int  precDefault: 0
    property int  precUser: 0
    property int  prec: precSrc == LimitsSource.Channel ? precChannel : (precSrc == LimitsSource.Default ? precDefault : precUser)
}
