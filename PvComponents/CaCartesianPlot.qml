import QtQuick 2.0

import PvComponents 1.0

Item {
    id: control

    property string title
    property string xLabel
    property string yLabel
    property color foreground: 'black'
    property color background:  '#bbbbbb'

    property int mode: 0
    property int style: PlotStyle.Line

    property int count: 10
    property string countPvName

    property string triggerPvName
    property string erasePvName
    property int eraseMode: 0

    property real xRangeLower: 5100.0
    property real xRangeUpper: 5400.0
    property int  xRangeMode: 0

    property real yRangeLower:-1.0
    property real yRangeUpper: 1.0
    property int  yRangeMode: 0

    property real y2RangeLower: 0.0
    property real y2RangeUpper: 1.0
    property int  y2RangeMode: 0

    QtObject {
        id: d
        property var pvCount
        property var pvTrigger
        property var pvErase
        property var pvs: []
        property var graphs: []
        property var xdata: []
        property var ydata: []
    }

    property ListModel models: ListModel {
        ListElement {
            xchannel: 'x'
            ychannel: 'y'
            foreground: 'red'
        }
    }

    Plot {
        id: plot
        anchors.fill: parent
        title: control.title
        foreground: control.foreground
        background: control.background
        property var xAxis: Axis {
            type: Axis.Bottom
            rangeLower: control.xRangeLower
            rangeUpper: control.xRangeUpper
        }
        property var yAxis: Axis {
            type: Axis.Left
            rangeLower: control.yRangeLower
            rangeUpper: control.yRangeUpper
        }
        property var y2Axis: Axis {
            type: Axis.Left
            rangeLower: control.y2RangeLower
            rangeUpper: control.y2RangeUpper
            visible: false
        }
        Axis {
            type: Axis.Top
            tickVisible: false
        }
        Axis {
            type: Axis.Right
            tickVisible: false
        }
    }

    Component.onCompleted: {
        if (countPvName != '') {
            d.pvCount = Qt.createQmlObject('import PvComponents 1.0; PvObject{channel: "%1"}'.arg(countPvName), control, 'pvCount')
            d.pvCount.valueChanged.connect(updateCount)
        }

        for(var i=0; i<models.count; i++) {
            var xchannel = models.get(i).xchannel
            var ychannel = models.get(i).ychannel
            // the first plot uses the first y axis
            // the second and following use the second y axis
            var xAxis = plot.xAxis
            var yAxis = plot.yAxis
            if (i > 0)
                yAxis = plot.y2Axis
            var graph = plot.addGraph(xAxis, yAxis)
            graph.color = models.get(i).foreground

            var xpv = null, ypv = null
            if (xchannel && xchannel != '') {
                xpv = Qt.createQmlObject(
                            'import PvComponents 1.0\n' +
                            'PvObject {\n' +
                            '    channel: "%1"\n'.arg(xchannel) +
                            '    property var data: new Array(%1)\n'.arg(count) +
                            '    onValueChanged: {\n' +
                            '        if (count == 1) { data.push(value); data.shift();}\n' +
                            '    }\n' +
                            '}',
                            control, 'xpv'+i)
                xpv.valueChanged.connect(updateData)
            }
            if (ychannel && ychannel != '') {
                ypv = Qt.createQmlObject('import PvComponents 1.0; PvObject{channel: "%1";property var data:[]}'.arg(ychannel), control, 'ypv'+i)
                ypv.valueChanged.connect(updateData)
            }
            d.pvs.push([xpv,ypv])
            d.graphs.push(graph)
        }
    }

    function updateCount() {
        count = d.pvCount.value
    }

    function updateData() {
        for (var i=0; i<d.pvs.length; i++) {
            var xpv = d.pvs[i][0]
            var ypv = d.pvs[i][1]
            var graph = d.graphs[i]
            var xv = null
            if (xpv != null) {
                if (xpv.count == 1) {
                    xv = xpv.data
                } else
                    xv = xpv.value
            }

            var yv = null
            if (ypv != null) {
                if (ypv.count == 1) {
                    yv = ypv.data
                } else
                    yv = ypv.value
            }
            graph.setData(xv, yv)
        }
        plot.replot()
    }
}
