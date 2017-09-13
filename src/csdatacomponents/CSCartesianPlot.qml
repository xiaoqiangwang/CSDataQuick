import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

import CSDataQuick.Components 1.0

/*!
    \qmltype CSCartesianPlot
    \inqmlmodule CSDataQuick.Components
    \brief Display XY Plot

    A Cartesian Plot consists of an X and one or two Y axes on which data can be plotted.
    The sets of data are called traces and consist of a set of (x, y) pairs, a color, and a Y axis indicator.
    The traces correspond to curves on the plot.

    Currently there can be up to eight traces on a plot.  The first trace is plotted on the Y1 axis,
    and the remaining traces (if any) are all plotted on the Y2 axis.  But the Y axis, Y or Y2, can be selected for each trace.
    The X and Y1 axes may have a label, but the Y2 axis does not.
    Each trace can (but does not have to) have a CSData from which to get the x values and
    another from which to get the y values.  These CSData can be of array type,
    such as Waveforms record in EPICS, or they can be of scalar type with only a single value.

    There are eight possible kinds of traces as seen in the following table.
    The traces for a given plot do not all have to be of the same kind – they can be mixed.

    In the table Nx is the number of elements in the CSData specified for x,
    and Ny is the number of elements in the CSData specified for y.
    The letter n denotes a number greater than one, and a blank indicates no CSData is specified.
    The axis limits LOPR and HOPR denote the limits obtained from CSData.
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

    Mouse interactions include,
    \table
    \row
        \li Click
        \li Select the trace for zooming/panning.
    \row
        \li Double Click
        \li Reset the zoom
    \row
        \li Wheel
        \li Zoom the selected trace's axes. If the mouse hovers over a certain axis, only that axis gets zoomed.
    \row
        \li Drag
        \li Pan view of the selected trace.
    \endtable

    \image cartesianplot.png
*/

BaseItem {
    id: root
    /*! This property holds the graph title */
    property string title
    /*! This property holds the X axis label */
    property string xLabel
    /*! This property holds the Y axis label */
    property string yLabel
    /*! This property indicates whether the legend is visible. */
    property bool legendVisible: false
    /*!
        \list
            \li false: Plot n points corresponding to the first n changes
                of the data, then do not plot any more points.
            \li true: Plot n points corresponding to the last n changes
                of the data, overwriting previous points.
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
    /*! Source from where to get number of points */
    property string countSource
    /*! Source of the trigger. If configured, whenever the value of the trigger data
    changes, the entire plot will be updated. */
    property string triggerSource
    /*!
    The erase source is a CSData object that causes erasing of the plot.
    If there is an erase source, the plot erases when the CSData value
    turns either zero or non-zero, depending on the eraseMode.
    */
    property string eraseSource
    /*!
    \list
        \li 0: Erase the plot if the erase-channel CSData is not zero.
        \li 1: Erase the plot if the erase-channel CSData is zero.
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

    implicitWidth: 300
    implicitHeight: 200
    background: ColorMap.monitors_background
    foreground: ColorMap.foreground

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

        The model can be ListModel or JSON object array. In any case, each element contains the following properties,
        \list
        \li label - trace label
        \li xchannel - data source for x
        \li ychannel - data source for y
        \li color - line color
        \endlist

        \e{Note: Only in the form of JSON object array, it can be used in Qt Quick Designer}
    */
    property var model

    ListModel {
        id: traceModel
    }

    onModelChanged: generateModel()
    /*! \internal */
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

    Plot {
        id: plot
        anchors.fill: parent
        title: root.title
        foreground: root.foreground
        background: root.background
        legendVisible: root.legendVisible
        property var xAxis: Axis {
            type: Axis.Bottom
            label: root.xLabel
            scale: xAxisStyle
            rangeLower: root.xRangeLower
            rangeUpper: root.xRangeUpper
            autoScale: xRangeStyle != CartesianPlotRangeStyle.User
        }
        property var yAxis: Axis {
            type: Axis.Left
            label: root.yLabel
            scale: yAxisStyle
            rangeLower: root.yRangeLower
            rangeUpper: root.yRangeUpper
            autoScale: yRangeStyle != CartesianPlotRangeStyle.User
        }
        property var y2Axis: Axis {
            type: Axis.Left
            scale: y2AxisStyle
            rangeLower: root.y2RangeLower
            rangeUpper: root.y2RangeUpper
            autoScale: y2RangeStyle != CartesianPlotRangeStyle.User
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

        onContextMenuRequested: contextMenu.popup()
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: 'Axes Range'
            onTriggered: dialog.open()
        }
    }

    ListModel {
        id: sourceModel
        ListElement {
            text: 'Channel';
            value: CartesianPlotRangeStyle.Channel;
        }
        ListElement {
            text: 'User';
            value: CartesianPlotRangeStyle.User;
        }
        ListElement {
            text: 'Auto';
            value: CartesianPlotRangeStyle.Auto;
        }
    }

    Dialog {
        id: dialog
        title: 'Define Axis Range'

        contentItem: GridLayout {
            anchors.fill: parent
            columns: 4
            rowSpacing: 2
            anchors.margins: 5

            Text {
                text: 'X'
                horizontalAlignment: Text.AlignRight
            }
            ComboBox {
                model: sourceModel
                textRole: "text"
                currentIndex: xRangeStyle
                onCurrentIndexChanged: {
                    xRangeStyle = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                implicitWidth: 80
                text: xRangeLower
                validator: DoubleValidator {}
                enabled: xRangeStyle == CartesianPlotRangeStyle.User
                onAccepted: xRangeLower = parseFloat(text)
            }
            TextField {
                implicitWidth: 80
                text: xRangeUpper
                validator: DoubleValidator {}
                enabled: xRangeStyle == CartesianPlotRangeStyle.User
                onAccepted: xRangeUpper = parseFloat(text)
            }

            Text {
                text: 'Y'
                horizontalAlignment: Text.AlignRight
            }
            ComboBox {
                model: sourceModel
                textRole: "text"
                currentIndex: yRangeStyle
                onCurrentIndexChanged: {
                    yRangeStyle = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                implicitWidth: 80
                text: yRangeLower
                validator: DoubleValidator {}
                enabled: yRangeStyle == CartesianPlotRangeStyle.User
                onAccepted: yRangeLower = parseFloat(text)
            }
            TextField {
                implicitWidth: 80
                text: yRangeUpper
                validator: DoubleValidator {}
                enabled: yRangeStyle == CartesianPlotRangeStyle.User
                onAccepted: yRangeUpper = parseFloat(text)
            }
            Text {
                text: 'Y2'
                horizontalAlignment: Text.AlignRight
            }
            ComboBox {
                model: sourceModel
                textRole: "text"
                currentIndex: y2RangeStyle
                onCurrentIndexChanged: {
                    y2RangeStyle = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                implicitWidth: 80
                text: y2RangeLower
                validator: DoubleValidator {}
                enabled: y2RangeStyle == CartesianPlotRangeStyle.User
                onAccepted: y2RangeLower = parseFloat(text)
            }
            TextField {
                implicitWidth: 80
                text: y2RangeUpper
                validator: DoubleValidator {}
                enabled: y2RangeStyle == CartesianPlotRangeStyle.User
                onAccepted: y2RangeUpper = parseFloat(text)
            }
            Button {
                id: ok
                text: 'Done'
                onClicked: {
                    dialog.accepted()
                    dialog.close()
                }
            }
        }
    }

    onCountChanged: {
        d.range.length = 0
        for(var j=0; j<count; j++){
            d.range.push(j)
        }
    }

    Component.onCompleted: {
        if (countSource != '') {
            d.pvCount = Qt.createQmlObject('import CSDataQuick.Data 1.0; CSData{source: "%1"}'.arg(countSource), root, 'pvCount')
            d.pvCount.valueChanged.connect(updateCount)
        }

        if (eraseSource != '') {
            d.pvErase = Qt.createQmlObject('import CSDataQuick.Data 1.0; CSData{source: "%1"}'.arg(eraseSource), root, 'pvErase')
            d.pvErase.valueChanged.connect(eraseGraph)
        }

        if (traceModel.count > 1)
            plot.y2Axis.visible = true
        for(var i=0; i<traceModel.count; i++) {
            var xchannel = traceModel.get(i).xchannel
            var ychannel = traceModel.get(i).ychannel
            // the first plot uses the first y axis
            // the second and following use the second y axis
            var xAxis = plot.xAxis
            var yAxis = traceModel.get(i).yaxis ? plot.y2Axis : plot.yAxis
            var graph = plot.addGraph(xAxis, yAxis)
            graph.name = Qt.binding(function() {return traceModel.get(i).label;})
            graph.color = Qt.binding(function() {return traceModel.get(i).color ? traceModel.get(i).color : 'black';})
            graph.lineStyle = Qt.binding(function () {return root.plotStyle;})

            var xpv = null, ypv = null
            if (xchannel) {
                xpv = Qt.createQmlObject(
                            'import CSDataQuick.Data 1.0\n' +
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
                            'import CSDataQuick.Data 1.0\n' +
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
                if (d.pvs[i][0])
                    d.pvs[i][0].data.length = 0
                if (d.pvs[i][1])
                    d.pvs[i][1].data.length = 0
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
