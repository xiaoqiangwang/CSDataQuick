import QtQuick 2.1
import QtQuick.Window 2.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

Window {
    id: app
    width: 900
    height: 800
    color: ColorMap.windows_background
    title: 'Demo of CSDataQuick.Components'

    Column {
        anchors.fill: parent
        spacing: 20

        Text {text: 'Graphics'}
        Flow {
            x: 10
            spacing: 10

            CSText {
                width: 180
                text: "Oh, A static label"
            }

            CSRect {
                width: 90
                height: 45
                fillStyle: FillStyle.Solid
            }

            CSOval {
                width: 60
                height: 80
                dynamicAttribute.channel: 'bo'
                colorMode: ColorMode.Alarm
                dynamicAttribute.visibilityMode: VisibilityMode.IfZero
                fillStyle: FillStyle.Outline
                lineWidth: 2
            }
            CSArc {
                CSData {
                    id: pv
                    source: 'catest'
                }
                width: 80
                height: 80
                begin: 90
                span: pv.value * 360
                fillStyle: FillStyle.Solid
            }
            CSPolygon {
                width: 100
                height: 50
                lineWidth: 2
                foreground: 'pink'
                fillStyle: FillStyle.Solid
                edgeStyle: EdgeStyle.Solid
                points: [Qt.point(50, 5), Qt.point(20, 40), Qt.point(80, 40)]
            }

            CSPolyline {
                width: 120
                height: 80
                lineWidth: 2
                foreground: 'black'
                background: 'black'
                edgeStyle: EdgeStyle.Solid
                points: [Qt.point(10,10),Qt.point(100,30),Qt.point(100,70),Qt.point(50,80)]
            }

            CSImage {
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

            CSSlider {
                source: 'catest'
                stepSize: 0.1
                labelStyle: LabelStyle.Channel
            }

            CSWheelSwitch {
                source: 'catest'
            }

            CSSpinBox {
                source: 'catest'
                unitsVisible: true
            }

            CSMessageButton {
                text: 'Click Me!'
                pressMessage: "1"
                source: 'calc'
                foreground: 'yellow'
                releaseMessage: "2"
            }

            CSTextEntry {
                width: 80
                limits.precDefault: 3
                limits.precSrc: LimitsSource.Default
                format: TextFormat.Compact
                source: 'catest'
                colorMode: ColorMode.Alarm
            }
            CSTextEntry {
                width: 80
                source: 'calc.SCAN'
            }

            CSMenu {
                width: 100
                source: 'calc.SCAN'
            }

            CSChoiceButton {
                width: 100
                stacking: Stacking.Row
                source: 'bo'
            }

            CSToggleButton {
                source: 'bo'
                text: 'Check'
            }

            CSRelatedDisplay {
                width: 80
                label: 'More'
                model: ListModel {
                    ListElement {label: 'test'; file: 'widgets.qml'; macro: ''; replace: true}
                    ListElement {label: 'controls'; file: 'controls.adl'; macro: ''; replace: false}
                }
            }

            CSShellCommand {
                label: 'list'
                model: ListModel {
                    ListElement {label: 'List Directory'; command: 'ls -l' }
                }
            }

            CSTextArea {
                source: 'wavec'
            }

            CSTextEntryArray {
                width: 50
                source: 'waved'
                count: 4
            }

        }
        Text {text: 'Monitor'}
        Flow {
            x: 10
            spacing: 10
            width: parent.width

            CSTextUpdate {
                source: 'calc'
            }

            CSTextUpdate {
                source: 'calc.SCAN'
            }

            CSByte {
                source: 'calc'
            }
            CSBar {
                limits.hoprDefault: 6
                limits.hoprSrc: LimitsSource.Default
                source: 'catest'
                colorMode: ColorMode.Alarm
                labelStyle: LabelStyle.Limits
            }
            CSIndicator {
                source: 'catest'
                colorMode: ColorMode.Alarm
                labelStyle: LabelStyle.Channel
            }

            CSMeter {
                source: 'catest'
            }

            CSStripChart {
                model: ListModel {
                    ListElement {
                        channel: 'catest'
                    }
                    ListElement {

                    }
                }
            }
            CSCartesianPlot {
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
