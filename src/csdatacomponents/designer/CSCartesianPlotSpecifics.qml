import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0

Column {
    anchors.left: parent.left
    anchors.right: parent.right

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Cartesian Plot")

        SectionLayout {
            Label {
                text: qsTr("title")
                tooltip: qsTr("graph title")
            }
            LineEdit {
                backendValue: backendValues.title
                showTranslateCheckBox: false
            }
            Label {
                text: qsTr("x label")
                tooltip: qsTr("graph x axis label")
            }
            LineEdit {
                backendValue: backendValues.xLabel
                showTranslateCheckBox: false
            }
            Label {
                text: qsTr("y label")
                tooltip: qsTr("graph y axis label")
            }
            LineEdit {
                backendValue: backendValues.yLabel
                showTranslateCheckBox: false
            }
            Label {
                text: qsTr("legend visible")
                tooltip: qsTr("graph y axis label")
            }
            CheckBox {
                backendValue: backendValues.legendVisible
            }
            Label {
                text: qsTr("count source")
                tooltip: qsTr("data count source")
            }
            LineEdit {
                backendValue: backendValues.countSource
                showTranslateCheckBox: false
            }
            Label {
                text: qsTr("plot style")
                tooltip: qsTr("plot style")
            }
            ComboBoxFix {
                backendValue: backendValues.plotStyle
                scope: 'PlotStyle'
                model: ['Point', 'Line', 'FillUnder']
            }
        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Axis X")

        SectionLayout {
            Label {
                text: qsTr("range")
                tooltip: qsTr("axis range")
            }
            SecondColumnLayout {
                ComboBoxFix {
                    id: rangeX
                    backendValue: backendValues.xRangeStyle
                    scope: 'CartesianPlotRangeStyle'
                    model: ['Channel', 'User', 'Auto']
                }
                SpinBox {
                    backendValue: backendValues.xRangeLower
                    minimumValue: -Infinity
                    maximumValue: Infinity
                    decimals: 2
                    enabled: rangeX.currentIndex == 1
                }
                SpinBox {
                    backendValue: backendValues.xRangeUpper
                    minimumValue: -Infinity
                    maximumValue: Infinity
                    decimals: 2
                    enabled: rangeX.currentIndex == 1
                }
            }
            Label {
                text: qsTr("style")
                tooltip: qsTr("axis style")
            }
            ComboBoxFix {
                backendValue: backendValues.xAxisStyle
                scope: 'CartesianPlotAxisStyle'
                model: ['Linear', 'Log10', 'Time']
            }
            Label {
                text: qsTr("time format")
            }
            ComboBoxFix {
                backendValue: backendValues.xTimeFormat
                scope: 'CartesianPlotTimeFormat'
                model: ['HHMMSS', 'HHMM', 'HH00', 'MMMDDYYYY', 'MMMDD', 'MMDDHH00', 'WDHH00']
            }
        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Axis Y")

        SectionLayout {
            Label {
                text: qsTr("range")
                tooltip: qsTr("axis range")
            }
            SecondColumnLayout {
                ComboBoxFix {
                    id: rangeY
                    backendValue: backendValues.yRangeStyle
                    scope: 'CartesianPlotRangeStyle'
                    model: ['Channel', 'User', 'Auto']
                }
                SpinBox {
                    backendValue: backendValues.yRangeLower
                    minimumValue: -Infinity
                    maximumValue: Infinity
                    decimals: 2
                    enabled: rangeY.currentIndex == 1
                }
                SpinBox {
                    backendValue: backendValues.yRangeUpper
                    minimumValue: -Infinity
                    maximumValue: Infinity
                    decimals: 2
                    enabled: rangeY.currentIndex == 1
                }
            }
            Label {
                text: qsTr("style")
                tooltip: qsTr("axis style")
            }
            ComboBoxFix {
                backendValue: backendValues.yAxisStyle
                scope: 'CartesianPlotAxisStyle'
                model: ['Linear', 'Log10', 'Time']
            }
        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Axis Y2")

        SectionLayout {
            Label {
                text: qsTr("range")
                tooltip: qsTr("axis range")
            }
            SecondColumnLayout {
                ComboBoxFix {
                    id: rangeY2
                    backendValue: backendValues.y2RangeStyle
                    scope: 'CartesianPlotRangeStyle'
                    model: ['Channel', 'User', 'Auto']
                }
                SpinBox {
                    backendValue: backendValues.y2RangeLower
                    minimumValue: -Infinity
                    maximumValue: Infinity
                    decimals: 2
                    enabled: rangeY2.currentIndex == 1
                }
                SpinBox {
                    backendValue: backendValues.y2RangeUpper
                    minimumValue: -Infinity
                    maximumValue: Infinity
                    decimals: 2
                    enabled: rangeY2.currentIndex == 1
                }
            }
            Label {
                text: qsTr("style")
                tooltip: qsTr("axis style")
            }
            ComboBoxFix {
                backendValue: backendValues.y2AxisStyle
                scope: 'CartesianPlotAxisStyle'
                model: ['Linear', 'Log10']
            }
        }
    }

    Section {
        anchors.left: parent.left
        anchors.right: parent.right
        caption: qsTr("Cartesian Plot Entry")

        CartesianPlotEditor {
            backendValue: backendValues.model
        }
    }
    ColorSection {}
}
