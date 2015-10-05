import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaCartesianPlot
    \inqmlmodule PvComponents
    \brief Display XY Plot

    A Cartesian Plot consists of an X and one or two Y axes on which data can be plotted.
    The sets of data are called traces and consist of a set of (x, y) pairs, a color, and a Y axis indicator.
    The traces correspond to curves on the plot.

    Currently there can be up to eight traces on a plot.  The first trace is plotted on the Y1 axis,
    and the remaining traces (if any) are all plotted on the Y2 axis.  But the Y axis, Y or Y2, can be selected for each trace.
    The X and Y1 axes may have a label, but the Y2 axis does not.
    Each trace can (but does not have to) have a process variable from which to get the x values and
    another from which to get the y values.  These process variables can be array process variables,
    such as Waveforms, or they can be scalar process variables with only a single value.

    There are eight possible kinds of traces as seen in the following table.
    The traces for a given plot do not all have to be of the same kind – they can be mixed.

    In the table Nx is the number of elements in the process variable specified for x,
    and Ny is the number of elements in the process variable specified for y.
    The letter n denotes a number greater than one, and a blank indicates no process variable is specified.
    The axis limits LOPR and HOPR denote the limits obtained from Channel Access for the process variable.
    Typically, these are the fields LOPR and HOPR in the associated record.
    Count is the specified Count for the Cartesian Plot, which is described in more detail below.

    \table
    \header
        \li Nx
        \li Ny
        \li Type
        \li Points
    \row
        \li n
        \li n
        \li X Vector, Y Vector
        \li x(i), y(i)
     \row
        \li n
        \li 1
        \li X Vector, Y Scalar
        \li x(i), y
    \row
        \li 1
        \li n
        \li X Scalar, Y Vector
        \li x, y(i)
    \row
        \li n
        \li
        \li X Vector
        \li x(i), i
    \row
        \li
        \li n
        \li Y Vector
        \li i, y(i)
    \row
        \li 1
        \li 1
        \li X Scalar, Y Scalar
        \li x, y
    \row
        \li 1
        \li
        \li X Scalar
        \li x, i
    \row
        \li
        \li 1
        \li Y Scalar
        \li i, y
    \endtable
*/

Item {
    id: root

    /*! Graph title */
    property string title
    /*! X axis label */
    property string xLabel
    /*! Y axis label */
    property string yLabel
    /*! foreground color */
    property color foreground: ColorMap.color14
    /*! background color */
    property color background: ColorMap.color4

    /*!
        \list
            \li false: Plot n points corresponding to the first n changes
                of the process variable, then do not plot any more points.
            \li true: Plot n points corresponding to the last n changes
                of the process variable, overwriting previous points.
        \endlist
    */
    property bool eraseOldest: false
    /*!
        \qmlproperty enumeration plotStyle
        \list
            \li 0: Plot the data as points.
            \li 1: Plot the data as lines.
            \li 2: Plot the data as lines which are filled under (or over)
                from the line to the axis.
        \endlist
    */
    property int plotStyle: PlotStyle.Point

    /*! Number of points */
    property int count: 0
    /*! Name of the PV from where to get number of points */
    property string countPvName

    /*! Name of the trigger PV. If configured, whenever the value of the trigger PV
    changes, the entire plot will be updated. */
    property string triggerPvName
    /*!
    The Erase Channel is a process variable that causes erasing of the plot.
    If there is an Erase Channel, the plot erases when the process variable
    turns either zero or non-zero, depending on the Erase Mode.
    */
    property string erasePvName
    /*!
    \list
        \li 0: Erase the plot if the erase-channel process variable is not zero.
        \li 1: Erase the plot if the erase-channel process variable is zero.
    \endlist
    */
    property int eraseMode: EraseMode.IfNotZero

    /*! \internal */
    property real xRangeLower: 0.0
    /*! \internal */
    property real xRangeUpper: 1.0
     /*! \internal */
    property int  xRangeStyle: CartesianPlotRangeStyle.Channel
    /*! \internal */
    property int  xAxisStyle: CartesianPlotAxisStyle.Linear
    /*! \internal */
    property int  xTimeFormat: CartesianPlotTimeFormat.HHMMSS


    /*! \internal */
    property real yRangeLower: 0.0
    /*! \internal */
    property real yRangeUpper: 1.0
    /*! \internal */
    property int  yRangeStyle:  CartesianPlotRangeStyle.Channel
    /*! \internal */
    property int  yAxisStyle: CartesianPlotAxisStyle.Linear
    /*! \internal */
    property int  yTimeFormat: CartesianPlotTimeFormat.HHMMSS

     /*! \internal */
    property real y2RangeLower: 0.0
    /*! \internal */
    property real y2RangeUpper: 1.0
    /*! \internal */
    property int  y2RangeStyle:  CartesianPlotRangeStyle.Channel
    /*! \internal */
    property int  y2AxisStyle: CartesianPlotAxisStyle.Linear
    /*! \internal */
    property int  y2TimeFormat: CartesianPlotTimeFormat.HHMMSS


    QtObject {
        id: d
        property var pvCount
        property var pvTrigger
        property var pvErase
        property var pvs: []
        property var graphs: []
        property var range: []
    }

    /*!
        Trace list model.
    */
    property ListModel model: ListModel {
    }

    Plot {
        id: plot
        anchors.fill: parent
        title: root.title
        foreground: root.foreground
        background: root.background
        property var xAxis: Axis {
            type: Axis.Bottom
            scale: xAxisStyle
            rangeLower: root.xRangeLower
            rangeUpper: root.xRangeUpper
        }
        property var yAxis: Axis {
            type: Axis.Left
            scale: yAxisStyle
            rangeLower: root.yRangeLower
            rangeUpper: root.yRangeUpper
        }
        property var y2Axis: Axis {
            type: Axis.Left
            scale: y2AxisStyle
            rangeLower: root.y2RangeLower
            rangeUpper: root.y2RangeUpper
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

    onCountChanged: {
        d.range.length = 0
        for(var j=0; j<count; j++){
            d.range.push(j)
        }
    }

    Component.onCompleted: {
        if (countPvName != '') {
            d.pvCount = Qt.createQmlObject('import PvComponents 1.0; CSData{source: "%1"}'.arg(countPvName), root, 'pvCount')
            d.pvCount.valueChanged.connect(updateCount)
        }

        if (erasePvName != '') {
            d.pvErase = Qt.createQmlObject('import PvComponents 1.0; CSData{source: "%1"}'.arg(erasePvName), root, 'pvErase')
            d.pvErase.valueChanged.connect(eraseGraph)
        }

        if (model.count > 1)
            plot.y2Axis.visible = true
        for(var i=0; i<model.count; i++) {
            var xchannel = model.get(i).xchannel
            var ychannel = model.get(i).ychannel
            // the first plot uses the first y axis
            // the second and following use the second y axis
            var xAxis = plot.xAxis
            var yAxis = model.get(i).yaxis ? plot.y2Axis : plot.yAxis
            var graph = plot.addGraph(xAxis, yAxis)
            graph.color = Qt.binding(function() {return model.get(i).foreground;})
            graph.lineStyle = Qt.binding(function () {return root.plotStyle;})

            var xpv = null, ypv = null
            if (xchannel) {
                xpv = Qt.createQmlObject(
                            'import PvComponents 1.0\n' +
                            'CSData {\n' +
                            '    source: "%1"\n'.arg(xchannel) +
                            '    property var data: []\n'+
                            '    onValueChanged: {\n' +
                            '        if (count == 1) { data.push(value); if (data.length > root.count) data.shift();}\n' +
                            '        updateData(%1)\n'.arg(i) +
                            '    }\n' +
                            '}',
                            root, 'xpv'+i)
            }
            if (ychannel) {
                ypv = Qt.createQmlObject(
                            'import PvComponents 1.0\n' +
                            'CSData {\n' +
                            '    source: "%1"\n'.arg(ychannel) +
                            '    property var data: []\n' +
                            '    onValueChanged: {\n' +
                            '        if (count == 1) { data.push(value); if (data.length > root.count) data.shift();}\n' +
                            '        updateData(%1)\n'.arg(i) +
                            '    }\n' +
                            '}',
                            root, 'ypv'+i)
            }
            d.pvs.push([xpv,ypv])
            d.graphs.push(graph)
        }
    }

    /*! \internal */
    function updateCount() {
        if (d.pvCount)
            count = Math.max(1, d.pvCount.value)
    }

    /*! \internal */
    function eraseGraph() {
        if ( (d.pvErase.value && root.eraseMode == EraseMode.IfNotZero) ||
             (d.pvErase.value == 0 && root.eraseMode == EraseMode.IfZero) ) {
            for (var i=0; i<d.graphs.length; i++) {
                d.graphs[i].clearData()
            }
            plot.replot()
        }
    }

    /*! \internal */
    function updateData(i) {
        var xpv = d.pvs[i][0]
        var ypv = d.pvs[i][1]
        var graph = d.graphs[i]
        var xv = null
        if (xpv) {
            if (xpv.count == 1) {
                if (ypv && ypv.count > 1)
                    xv = xpv.value
                else
                    xv = xpv.data
            } else {
                if (count == 0) count = xpv.count
                xv = xpv.value
            }
        } else {
            xv = d.range
        }

        var yv = null
        if (ypv) {
            if (ypv.count == 1) {
                if (xpv && xpv.count > 1)
                    yv = ypv.value
                else
                    yv = ypv.data
            } else {
                if (count == 0) count = ypv.count
                yv = ypv.value
            }
        } else
            yv = d.range

        graph.setData(xv, yv)

        switch (xRangeStyle) {
        case CartesianPlotRangeStyle.Channel:
        case CartesianPlotRangeStyle.Auto:
            plot.xAxis.rescale()
            break
        case CartesianPlotRangeStyle.User:
            plot.xAxis.rangeLower = root.xRangeLower
            plot.xAxis.rangeUpper = root.xRangeUpper
            break
        }

        switch (yRangeStyle) {
        case CartesianPlotRangeStyle.Channel:
        case CartesianPlotRangeStyle.Auto:
            plot.yAxis.rescale()
            break
        case CartesianPlotRangeStyle.User:
            plot.yAxis.rangeLower = root.yRangeLower
            plot.yAxis.rangeUpper = root.yRangeUpper
            break
        }

        switch (y2RangeStyle) {
        case CartesianPlotRangeStyle.Channel:
        case CartesianPlotRangeStyle.Auto:
            plot.y2Axis.rescale()
            break
        case CartesianPlotRangeStyle.User:
            plot.y2Axis.rangeLower = root.y2RangeLower
            plot.y2Axis.rangeUpper = root.y2RangeUpper
            break
        }

        plot.replot()
    }
}
