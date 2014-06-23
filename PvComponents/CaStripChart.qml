import QtQuick 2.0

import PvComponents 1.0

Item {
    id: control

    property string title
    property color foreground: 'black'
    property color background:  '#bbbbbb'
    property int units: TimeUnit.Second
    property int period: 60
    property ListModel models

    // list of created pvs and their corresponding graph and time,data
    QtObject {
        id: d
        property var pvs: []
        property var graphs: []
        property var time: []
        property var data: []
    }


    Plot {
        id: plot
        anchors.fill: parent
        title: control.title
        foreground: control.foreground
        background: control.background
        property var xAxis: Axis {
            type: Axis.Bottom
            label: "Time (%1)".arg(getTimeLabel(control.units))
            rangeLower: -period
            rangeUpper: 0
        }
        Axis {
            id: xAxis2
            type: Axis.Top
            tickVisible: false
        }
        Axis {
            id: yAxis2
            type: Axis.Right
            tickVisible: false
        }
    }

    Component.onCompleted: {
        var length = period * getInterval()
        for (var i=1; i<=length; i++) {
            d.time.push((i - length) / getInterval())
        }
        for(var i=0; i<models.count; i++) {
            if (models.get(i).channel == '')
                continue
            // create graph with its own left axis
            var axis = Qt.createQmlObject('import PvComponents 1.0; Axis {type: Axis.Left}', plot, 'axis' + i)
            var graph = plot.addGraph(plot.xAxis, axis)
            graph.color = models.get(i).foreground
            d.graphs.push(graph)
            // create pv object
            var cmd =
                    'import PvComponents 1.0\n' +
                    'PvObject {\n' +
                    '    channel: "%1"\n'.arg(models.get(i).channel) +
                    '    property var data\n' +
                    '    property Limits limits : Limits {\n' +
                    '        loprSrc: %1\n'.arg(models.get(i).loprSrc) +
                    '        loprDefault: %1\n'.arg(models.get(i).loprDefault) +
                    '        hoprSrc: %1\n'.arg(models.get(i).hoprSrc) +
                    '        hoprDefault: %1\n'.arg(models.get(i).hoprDefault) +
                    '    }\n' +
                    '    onConnectionChanged: {\n' +
                    '        if (connected) {\n' +
                    '            if (lodisplim < updisplim) {\n' +
                    '                limits.loprChannel = lodisplim\n' +
                    '                limits.hoprChannel = updisplim\n' +
                    '            }\n' +
                    '        }\n' +
                    '    }\n' +
                    '}'
            var pv = Qt.createQmlObject(cmd, control, 'pv'+i)
            d.pvs.push(pv)

            // bind pv range to y axis range
            axis.rangeLower = pv.limits.lopr
            axis.rangeUpper = pv.limits.hopr

            // fill data array with NaN and assign to pv object
            var data = []
            for (var j=0; j<length; j++) {
                data.push(Number.NaN)
            }
            pv.data = data
        }
        plot.replot()
    }
    Timer {
        id: timer
        running: true
        repeat: true
        interval: 1000
        onTriggered: {
            update()
        }
    }

    function getTimeLabel(units)
    {
        switch (units) {
            // we don't support ms
           case TimeUnit.MilliSecond:
               return "ms";
           case TimeUnit.Second:
               return "sec";
           case TimeUnit.Minute:
               return "min";
           default:
               return "s";
        }
    }

    function getInterval()
    {
        switch (units) {
            // we don't support ms
           case TimeUnit.MilliSecond:
               return 1;
           case TimeUnit.Second:
               return 1;
           case TimeUnit.Minute:
               return 60;
           default:
               return 1;
        }
    }

    function update() {
        for(var i=0; i<d.pvs.length; i++) {
            d.pvs[i].data.shift()
            if (d.pvs[i].connected)
                d.pvs[i].data.push(d.pvs[i].value)
            else
                d.pvs[i].data.push(Number.NaN)
            d.graphs[i].setData(d.time, d.pvs[i].data)
        }
        plot.replot()
    }
}
