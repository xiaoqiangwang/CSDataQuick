import QtQuick 2.1
import QtQuick.Controls 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

ApplicationWindow {
    id: app
    width: 900
    height: 800
    color: ColorMap.windows_background
    title: 'A demo of qml pv components'

    menuBar: MenuBar {
        Menu {
            title: 'File'
            MenuItem {
                text: 'Exit'
                onTriggered: app.close()
            }
        }
        Menu {
            title: 'Help'
            MenuItem {
                text: 'About'
                onTriggered: console.log('A demo of qml pv components')
            }
        }
    }

    Column {
        anchors.fill: parent
        spacing: 20

        Text {text: 'Graphics'}
        Flow {
            x: 10
            spacing: 10

            CaText {
                text: "Oh, A static label"
            }

            CaRect {
                fillStyle: FillStyle.Solid
            }

            CaOval {
                dynamicAttribute.channel: 'bo'
                colorMode: ColorMode.Alarm
                dynamicAttribute.visibilityMode: VisibilityMode.IfZero
                fillStyle: FillStyle.Outline
                lineWidth: 2
            }
            CaArc {
                CSData {
                    id: pv
                    source: 'catest'
                }
                begin: 90
                span: pv.value * 360
                fillStyle: FillStyle.Solid
            }
            CaPolygon {
                width: 100
                height: 50
                lineWidth: 2
                foreground: 'pink'
                fillStyle: FillStyle.Solid
                edgeStyle: EdgeStyle.Solid
                points: [Qt.point(50, 5), Qt.point(20, 40), Qt.point(80, 40)]
            }

            CaPolyline {
                width: 120
                height: 80
                lineWidth: 2
                foreground: 'black'
                background: 'black'
                edgeStyle: EdgeStyle.Solid
                points: [Qt.point(10,10), Qt.point(100, 30),
                    Qt.point(100, 70), Qt.point(50, 80)]
            }

            CaImage {
                source: 'LED.gif'
                dynamicAttribute.channel: 'catest'
                imageCalc: 'A*10'
            }
        }
        Text {text: 'Control'}
        Flow {
            x: 10
            spacing: 10
            width: parent.width

            CaSlider {
                source: 'catest'
                stepSize: 0.1
                labelStyle: LabelStyle.Channel
            }

            CaMessageButton {
                text: 'Click Me!'
                onMessage: "1"
                source: 'calc'
                foreground: 'yellow'
                offMessage: "2"
            }

            CaTextEntry {
                limits.precDefault: 3
                limits.precSrc: LimitsSource.Default
                format: TextFormat.Compact
                source: 'catest'
                colorMode: ColorMode.Alarm
            }
            CaTextEntry {
                source: 'calc.SCAN'
            }

            CaMenu {
                source: 'calc.SCAN'
            }

            CaChoiceButton {
                stacking: Stacking.Row
                source: 'bo'
            }

            CaRelatedDisplay {
                label: 'More'
                model: ListModel {
                    ListElement {label: 'test'; fname: 'widgets.qml'; args: ''; remove: false}
                }
            }

            CaShellCommand {
                label: 'list'
                model: ListModel {
                    ListElement {label: 'List Directory'; command: 'ls -l' }
                }
            }

        }
        Text {text: 'Monitor'}
        Flow {
            x: 10
            spacing: 10
            width: parent.width

            CaTextUpdate {
                source: 'calc'
            }

            CaTextUpdate {
                source: 'calc.SCAN'
            }

            CaByte {
                source: 'calc'
            }
            CaBar {
                limits.hoprDefault: 6
                limits.hoprSrc: LimitsSource.Default
                source: 'catest'
                colorMode: ColorMode.Alarm
                labelStyle: LabelStyle.Limits
            }
            CaBar {
                direction: Direction.Down
                labelStyle: LabelStyle.Outline
               source: 'catest'
               colorMode: ColorMode.Alarm
           }
            CaIndicator {
                source: 'catest'
                colorMode: ColorMode.Alarm
                labelStyle: LabelStyle.Channel
            }

            CaMeter {
                source: 'catest'
            }

            CaStripChart {
                model: ListModel {
                    ListElement {
                        channel: 'catest'
                    }
                    ListElement {

                    }
                }
            }
            CaCartesianPlot {
                model: ListModel{
                    ListElement {
                        xchannel: "x"
                        ychannel: "y"
                        foreground: "black"
                    }
                }
            }
        }
    }
}
