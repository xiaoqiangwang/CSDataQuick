import QtQuick 2.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

/*!
    \qmltype CaStripChart
    \inqmlmodule CSDataQuick.Components
    \brief Display a strip chart

*/

BaseItem {
    id: root
    width: 300
    height: 200
    background:  ColorMap.monitors_background
    foreground: ColorMap.foreground
    /*!
        The title of the plot, displayed on top.
    */
    property string title
    /*!
        The lable of x axis, shown below the axis.
    */
    property string xLabel
    /*!
        The label of y axis, show next to the axis.
    */
    property string yLabel
    /*!
        \qmlproperty enumeration units
        The time axis units.
    */
    property int units: TimeUnit.Second
    /*!
        The period is the time between updates
    */
    property int period: 60
    /*!
        The PV list model

        Each ListElement contains the following properties,
        \list
        \li channel  - the process variable to monitor.
        \li color - the color of the line for that channel,
        \li loprSrc - the lower range source.
        \li loprDefault - the default lower range.
        \li hoprSrc - the upper range source.
        \li hoprDefault - the default upper range.
        \endlist
    */
    property var model

    ListModel {
        id: traceModel
    }

    onModelChanged: generateModel()
    function generateModel() {
        if (model instanceof Array) {
            traceModel.clear()
            for(var i=0; i<model.length; i++)
                traceModel.append(model[i])
        } else {
            traceModel.clear()
            for(var i=0; i<model.count; i++)
                traceModel.append(model.get(i))
        }
    }

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
        title: root.title
        foreground: root.foreground
        background: root.background
        property var xAxis: Axis {
            type: Axis.Bottom
            label: xLabel ? xLabel : (yLabel ? "Time (%1)".arg(getTimeLabel(root.units)) : '')
            rangeLower: -period
            rangeUpper: 0
            autoScale: false
        }
        property var yAxis: Axis {
            type: Axis.Left
            autoScale: false
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

    Component.onCompleted: createTraces()

    function createTraces() {
        var length = period * getInterval()
        for (var i=1; i<=length; i++) {
            d.time.push((i - length) / getInterval())
        }
        for(var i=0; i<traceModel.count; i++) {
            var pen = traceModel.get(i);

            if (!pen.channel)
                continue
            // create graph with its own left axis
            var axis
            if (i == 0)
                axis = plot.yAxis
            else
                axis = Qt.createQmlObject('import CSDataQuick.Components 1.0; Axis {type: Axis.Left; autoScale: false}', plot, 'yaxis' + i)
            var graph = plot.addGraph(plot.xAxis, axis)
            if (pen.color)
                graph.color = pen.color
            d.graphs.push(graph)
            // create pv object
            var cmd =
                    'import CSDataQuick.Data 1.0\n' +
                    'import CSDataQuick.Components 1.0\n' +
                    'CSData {\n' +
                    '    source: "%1"\n'.arg(pen.channel) +
                    '    property var data\n' +
                    '    property Limits limits : Limits {\n';
            if (pen.loprSrc)
              cmd +='        loprSrc: %1\n'.arg(limitsEnumString(pen.loprSrc))
            if (pen.loprDefault)
              cmd +='        loprDefault: %1\n'.arg(pen.loprDefault)
            if (pen.hoprSrc)
              cmd +='        hoprSrc: %1\n'.arg(limitsEnumString(pen.hoprSrc))
            if (pen.hoprDefault)
              cmd +='        hoprDefault: %1\n'.arg(pen.hoprDefault)
              cmd +='    }\n' +
                    '    onConnectionChanged: {\n' +
                    '        if (connected) {\n' +
                    '            if (range.isValid()) {\n' +
                    '                limits.loprChannel = range.lower\n' +
                    '                limits.hoprChannel = range.upper\n' +
                    '            }\n' +
                    '        }\n' +
                    '    }\n' +
                    '}'
            var pv = Qt.createQmlObject(cmd, root, 'pv'+i)
            d.pvs.push(pv)

            // bind pv range to y axis range
            axis.rangeLower = Qt.binding(function() {return pv.limits.lopr;})
            axis.rangeUpper = Qt.binding(function() {return pv.limits.hopr;})

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

    /*! \internal */
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
    /*! \internal */
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
    /*! \internal */
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
    /* \internal */
    function limitsEnumString(index) {
        switch (index) {
            case 0:
            return 'Limits.Channel';
            case 1:
            return 'Limits.Default';
            case 2:
            return 'Limits.User';
            case 3:
            return 'Limits.Unused';
        }
    }
}
