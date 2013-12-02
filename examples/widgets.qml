import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0

ApplicationWindow {
    id: app
    width: 900
    height: 400
    color: '#bbbbbb'

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

        Text {text: 'Graphics'}
        Flow {
            x: 10
            spacing: 10

            CaText {
                width: 100
                height: 20
                text: 'Oh, A static label'
            }

            CaRect {
                width: 100
                height: 70
                foreground: 'blue'
                fill: FillStyle.Solid
            }

            CaOval {
                channel: 'bo'
                colorMode: ColorMode.Alarm
                width: 100
                height: 70
                fill: FillStyle.Solid
                lineWidth: 2
            }

            CaArc {
                PvObject {
                    id: pv
                    channel: 'catest'
                }
                width: 100
                height: 100
                begin: 0
                end: pv.value * 360
            }

            CaPolygon {
                width: 100
                height: 50
                lineWidth: 2
                foreground: 'pink'
                fill: FillStyle.Solid

                Component.onCompleted: {
                    points = create_sin()
                }
                function create_sin() {
                    var pts = []
                    for (var i=0; i<20; i++) {
                        x = width / 20 * i
                        y = height / 2.5 *Math.sin(i / 20. * 2 * Math.PI) + height / 2
                        pts.push(Qt.point(x, y))
                    }
                    return pts
                }
            }

            CaPolyline {
                width: 200
                height: 80
                channelC: ""
                lineWidth: 2
                foreground: 'black'
                Component.onCompleted: {
                    points = create_sin()
                }
                function create_sin() {
                    var pts = []
                    for (var i=0; i<20; i++) {
                        x = width / 20 * i
                        y = height / 2.5 *Math.sin(i / 20. * 4 * Math.PI) + height / 2
                        pts.push(Qt.point(x, y))
                    }
                    return pts
                }
            }

            CaImage {
                width: 100
                height: 100
                source: 'LED.gif'
                channel: 'catest'
                imageCalc: 'A * 10'
            }
        }
        Text {text: 'Control'}
        Flow {
            x: 10
            spacing: 10
            width: parent.width

            CaMessageButton {
                width: 100
                height: 24
                text: 'Click Me!'
                channel: 'catest'
                foreground: 'yellow'
                onMessage: 0.1
                offMessage: 0.5
            }

            CaSlider {
                width: 100
                height: 24
                channel: 'catest'
                stepSize: 0.01
            }

            CaTextEntry {
                width:100
                height:24
                channel: 'catest'
                colorMode: 'alarm'
                background: '#73dfff'
            }

            CaMenu {
                width: 100
                height: 50
                channel: 'calc.SCAN'
            }

            CaChoiceButton {
                width: 100
                orientation: 0
                channel: 'bo'
            }

            CaRelatedDisplay {
                width: 100
                height: 20
                label: 'More'
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

            CaTextLabel {
                channel: 'calc'
                width: 100
                height: 20
            }
            CaByte {
                width: 320
                height: 10
                visibilityCalc: ""
                channel: 'calc'
            }
            CaBar {
                width: 100
                height: 20
                channel: 'catest'
            }
            CaMeter {
                width: 100
                height: 100
                channel: 'catest'
            }
            CaStripChart {
                width: 500
                height: 300
                channel: 'catest'
            }
       }
    }
}
