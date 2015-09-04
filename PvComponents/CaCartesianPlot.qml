import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaCartesianPlot
    \inqmlmodule PvComponents
    \brief Display XY Plot

*/

Item {
    id: control

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
        \qmlproperty enumeration mode
        \list
            \li 0: Plot n points corresponding to the first n changes
                of the process variable, then do not plot any more points.
            \li 1: Plot n points corresponding to the last n changes
                of the process variable, overwriting previous points.
        \endlist
    */
    property int mode: 0
    /*!
        \qmlproperty enumeration style
        \list
            \li 0: Plot the data as points.
            \li 1: Plot the data as lines.
            \li 2: Plot the data as lines which are filled under (or over)
                from the line to the axis.
        \endlist
    */
    property int style: PlotStyle.Line

    /*! Number of points */
    property int count: 10
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
    property int eraseMode: 0

    /*! \internal */
    property real xRangeLower: 5100.0
    /*! \internal */
    property real xRangeUpper: 5400.0
     /*! \internal */
   property int  xRangeMode: 0

    /*! \internal */
    property real yRangeLower:-1.0
    /*! \internal */
    property real yRangeUpper: 1.0
    /*! \internal */
    property int  yRangeMode: 0

     /*! \internal */
    property real y2RangeLower: 0.0
    /*! \internal */
    property real y2RangeUpper: 1.0
    /*! \internal */
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

    /*!
        Trace list model.
    */
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

    /*! \internal */
    function updateCount() {
        count = d.pvCount.value
    }

    /*! internal */
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
