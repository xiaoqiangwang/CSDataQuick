import QtQuick 2.0

import CSDataQuick.Components 1.0

Item {
    width: 800
    height: 600
    //title: "vfdv"

    CaBar {
        id: bar1
        x: 213
        y: 226
        width: 57
        height: 149
        label: LabelStyle.Channel
        direction: Direction.Down
        source: "$(P)"
    }

    CaChoiceButton {
        id: choiceButton1
        x: 287
        y: 251
        width: 159
        height: 41
        source: "bo"
        background: "#73dfff"
        stacking: Stacking.Row
    }

    CaIndicator {
        id: indicator1
        x: 483
        y: 201
        width: 65
        height: 152
        source: "catest"
        label: LabelStyle.Limits
        direction: Direction.Up
    }

    CaOval {
        id: oval1
        x: 230
        y: 35
        width: 93
        height: 91
    }

    CaRect {
        id: rectangle1
        x: 403
        y: 65
        width: 151
        height: 67
        dynamicAttribute.channel: "bo"
        dynamicAttribute.visibilityMode: VisibilityMode.IfZero
        foreground: "#9e9e9e"
    }

    CaText {
        id: text1
        x: 22
        y: 234
        width: 159
        height: 17
        text: "This a static text"
    }

    CaMenu {
        id: menu1
        x: 25
        y: 192
        width: 82
        height: 23
        source: "bo"
    }

    CaMeter {
        id: meter1
        x: 309
        y: 391
        width: 156
        height: 89
        limits.hoprDefault: 5
        limits.hoprSrc: LimitsSource.Default
        label: LabelStyle.Outline
        source: "catest"
    }

    CaStripChart {
        id: stripChart1
        x: 60
        y: 391
        width: 238
        height: 169
        units: TimeUnit.Second
        yLabel: "dc"
        xLabel: "sd"
        title: "sas"
    }

    CaText {
        id: text2
        x: 328
        y: 490
        width: 144
        height: 40
        text: "Level"
        align: Text.AlignHCenter
        background: "#eeb62b"
    }

    CaArc {
        id: arc1
        x: 108
        y: 38
        width: 81
        height: 81
        begin: 80
    }

    CaPolyline {
        id: polyline1
        x: 37
        y: 35
        width: 58
        height: 86
        lineWidth: 6
        edge: EdgeStyle.Dash
        fill: FillStyle.Solid
        points: [Qt.point(12,20),Qt.point(13,74),Qt.point(36,21),Qt.point(43,71)]
    }

    CaWheelSwitch {
        id: wheelswitch1
        x: 113
        y: 184
        width: 76
        height: 40
        colorMode: ColorMode.Alarm
        source: "catest"
    }

    CaSlider {
        id: slider1
        x: 194
        y: 172
        width: 151
        height: 23
        source: "catest"
    }

}

