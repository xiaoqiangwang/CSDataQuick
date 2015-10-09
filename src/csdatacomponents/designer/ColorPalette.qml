import QtQuick 2.0
import QtQuick.Controls 1.0 as Controls
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.0

Column {
    id: colorPalette
    width: parent.width

    Layout.preferredWidth: 200
    Layout.preferredHeight: showPalette ? 100 : 20

    property variant backendValue
    property color valueFromBackend: backendValue.value

    property color color
    property bool showPalette: false

    onColorChanged: {
        if (backendValue === undefined)
            return

        if (backendValue.value != colorPalette.color)
            backendValue.value = colorPalette.color
    }

    onBackendValueChanged: {
        if (backendValue === undefined)
            return

        if (colorPalette.value != backendValue.value)
            colorPalette.color = backendValue.value
    }

    onValueFromBackendChanged: {
        if (colorPalette.value != backendValue.value)
            colorPalette.color = backendValue.value
    }

    Rectangle {
        id: button
        width: parent.width
        height: 20
        color: colorPalette.color
        MouseArea {
            anchors.fill: parent
            onClicked: {
                showPalette = !showPalette
            }
        }
    }

    Grid {
        id: grid
        columns: 13
        spacing: 1
        flow: Grid.TopToBottom

        visible: showPalette

        Controls.ExclusiveGroup {
            id: colorButtonGroup
        }
        Repeater {
            model: 65
            delegate: Controls.Button {
                width: 15
                height: 15
                style: ButtonStyle{
                    background: Rectangle {
                        color: colorPalette['color%1'.arg(index)]
                        border.width: checked ? 1 : 0
                    }
                }
                checkable: true
                exclusiveGroup: colorButtonGroup
                onClicked: {
                    colorPalette.color = color
                    colorPalette.showPalette = false
                }
                property color color: colorPalette['color%1'.arg(index)]
            }
        }
    }

    property color color0: "#ffffff"
    property color color1: "#ececec"
    property color color2: "#dadada"
    property color color3: "#c8c8c8"
    property color color4: "#bbbbbb"
    property color color5: "#aeaeae"
    property color color6: "#9e9e9e"
    property color color7: "#919191"
    property color color8: "#858585"
    property color color9: "#787878"
    property color color10: "#696969"
    property color color11: "#5a5a5a"
    property color color12: "#464646"
    property color color13: "#2d2d2d"
    property color color14: "#000000"
    property color color15: "#00d800"
    property color color16: "#1ebb00"
    property color color17: "#339900"
    property color color18: "#2d7f00"
    property color color19: "#216c00"
    property color color20: "#fd0000"
    property color color21: "#de1309"
    property color color22: "#be190b"
    property color color23: "#a01207"
    property color color24: "#820400"
    property color color25: "#5893ff"
    property color color26: "#597ee1"
    property color color27: "#4b6ec7"
    property color color28: "#3a5eab"
    property color color29: "#27548d"
    property color color30: "#fbf34a"
    property color color31: "#f9da3c"
    property color color32: "#eeb62b"
    property color color33: "#e19015"
    property color color34: "#cd6100"
    property color color35: "#ffb0ff"
    property color color36: "#d67fe2"
    property color color37: "#ae4ebc"
    property color color38: "#8b1a96"
    property color color39: "#610a75"
    property color color40: "#a4aaff"
    property color color41: "#8793e2"
    property color color42: "#6a73c1"
    property color color43: "#4d52a4"
    property color color44: "#343386"
    property color color45: "#c7bb6d"
    property color color46: "#b79d5c"
    property color color47: "#a47e3c"
    property color color48: "#7d5627"
    property color color49: "#58340f"
    property color color50: "#99ffff"
    property color color51: "#73dfff"
    property color color52: "#4ea5f9"
    property color color53: "#2a63e4"
    property color color54: "#0a00b8"
    property color color55: "#ebf1b5"
    property color color56: "#d4db9d"
    property color color57: "#bbc187"
    property color color58: "#a6a462"
    property color color59: "#8b8239"
    property color color60: "#73ff6b"
    property color color61: "#52da3b"
    property color color62: "#3cb420"
    property color color63: "#289315"
    property color color64: "#1a7309"
}

