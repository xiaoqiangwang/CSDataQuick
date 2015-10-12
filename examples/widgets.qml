import QtQuick 2.1
import QtQuick.Controls 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

ApplicationWindow {
    id: app
    width: 900
    height: 800
    color: '#bbbbbb'
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
                width: 200
                height: 20
                text: "Oh, A static label"
                align: Qt.AlignRight
            }

            CaRect {
                width: 100
                height: 70
                foreground: "#000000"
                fill: FillStyle.Solid
            }

            CaOval {
                dynamicAttr.channel: 'bo'
                dynamicAttr.channelB: 'catest'
                colorMode: ColorMode.Alarm
                width: 100
                height: 70
                fill: FillStyle.Solid
                lineWidth: 2
            }
            CaArc {
                CSData {
                    id: pv
                    source: 'catest'
                }

                width: 100
                height: 100
                begin: 90
                span: pv.value * 360
                fill: FillStyle.Solid
            }
            CaPolygon {
                width: 100
                height: 50
                lineWidth: 2
                foreground: 'pink'
                fill: FillStyle.Solid
                edge: EdgeStyle.Solid
                points: [Qt.point(50, 5), Qt.point(20, 40), Qt.point(80, 40)]
            }

            CaPolyline {
                width: 120
                height: 80
                lineWidth: 2
                foreground: 'black'
                background: 'black'
                edge: EdgeStyle.Solid
                points: [Qt.point(10,10), Qt.point(100, 30),
                    Qt.point(100, 70), Qt.point(50, 80)]
            }

            CaImage {
                width: 80
                height: 80
                source: 'LED.gif'
                dynamicAttr.channel: 'catest'
                imageCalc: 'A*10'
            }
        }
        Text {text: 'Control'}
        Flow {
            x: 10
            spacing: 10
            width: parent.width

            CaSlider {
                width: 200
                height: 50
                source: 'catest'
                stepSize: 0.1
                label: LabelStyle.Channel
            }

            CaMessageButton {
                width: 100
                height: 24
                text: 'Click Me!'
                source: 'calc'
                foreground: 'yellow'
                onMessage: 0.1
                offMessage: 0.5
            }

            CaTextEntry {
                width:100
                height:24
                source: 'catest'
                colorMode: ColorMode.Alarm
            }
            CaTextEntry {
                width: 200
                height: 20
                source: 'calc.SCAN'
            }

            CaMenu {
                width: 100
                height: 20
                source: 'calc.SCAN'
            }

            CaChoiceButton {
                width: 100
                height: 30
                stacking: Stacking.Row
                source: 'bo'
            }

            CaRelatedDisplay {
                width: 100
                height: 20
                label: 'More'
                model: ListModel {
                    ListElement {label: 'test'; fname: 'widgets.qml'; args: ''; remove: false}
                }
            }

            CaShellCommand {
                width: 100
                height: 20
                label: 'list'
                model: ListModel {
                    ListElement {label: 'List Directory'; command: 'ls'; args: '-l'; }
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
                width: 100
                height: 20
            }

            CaTextUpdate {
                source: 'calc.SCAN'
                width: 150
                height: 20
            }

            CaByte {
                width: 320
                height: 15
                source: 'calc'
            }
            CaBar {
                width: 100
                height: 40
                source: 'catest'
                colorMode: ColorMode.Alarm
                label: LabelStyle.Limits
            }
            CaBar {
               width: 50
               height: 100
               direction: Direction.Down
               label: LabelStyle.Outline
               source: 'catest'
               colorMode: ColorMode.Alarm
           }
            CaIndicator {
                width: 100
                height: 50
                source: 'catest'
                colorMode: ColorMode.Alarm
                label: LabelStyle.Channel
            }

            CaMeter {
                width: 80
                height: 60
                source: 'catest'
            }

            CaStripChart {
                width: 300
                height: 200
                model: ListModel {
                    ListElement {
                        channel: 'catest'
                    }
                    ListElement {

                    }
                }
            }
            CaCartesianPlot {
                width: 300
                height: 200
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
