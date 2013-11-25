import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0

ApplicationWindow {
    id: app
    width: 800
    height: 400

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
                text: 'A static label'
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
                start: 0
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
                width: 100
                height: 50
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
        }
        Text {text: 'Control'}
        Flow {
            x: 10
            spacing: 10
            CaMessageButton {
                width: 100
                height: 24
                text: 'Click Me!'
                channel: 'catest'
                foreground: 'yellow'
                background: 'steelblue'
                onMessage: 0.1
                offMessage: 0.5
            }

            CaSlider {
                width: 100
                height: 24
                channel: 'catest'
            }

            CaTextEntry {
                width:100
                height:24
                channel: 'catest'
                colorMode: 'alarm'
            }
            CaMenu {
                width: 100
                height: 20
                channel: 'calc.SCAN'
            }

            CaChoiceButton {
                width: 100
                orientation: 0
                channel: 'bo'
                background: 'lightsteelblue'
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
       }
    }
}
