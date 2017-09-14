import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

/*!
    \qmltype CSStripChart
    \inqmlmodule CSDataQuick.Components
    \brief Display a strip chart

    A strip chart acquires the data and updates the plot in fixed period.
    \l period and \l units together define the time span.

    \qml
    CSStripChart {
        model: ListModel {
            ListElement {
                channel: 'catest'
                color: 'red'
            }
            ListElement {
                channel: 'catest2'
                color: 'blue'
            }
        }
    }
    \endqml

    \image stripchart.png
*/

BaseItem {
    id: root

    /*! This property holds the title of the plot, displayed on top. */
    property string title
    /*! This property holds the lable of x axis, shown below the axis. */
    property string xLabel
    /*! This property holds the label of y axis, show next to the axis. */
    property string yLabel
    /*! This property indicates whether the legend is visible. */
    property bool legendVisible: false
    /*!
        \qmlproperty enumeration units
        This property holds the time axis units.

        \list
        \li TimeUnit.MilliSecond
        \li TimeUnit.Second
        \li TimeUnit.Minute
        \endlist
    */
    property int units: TimeUnit.Second
    /*! This property holds the period of time between updates */
    property int period: 60
    /*!
        This property holds the data list model.

        The model can be ListModel or JSON object array. In any case, each element contains the following properties,
        \list
        \li channel  - the data source to monitor.
        \li label - the descriptive text.
        \li color - the color of the line for that channel,
        \li loprSrc - the lower range source.
        \li loprDefault - the default lower range.
        \li hoprSrc - the upper range source.
        \li hoprDefault - the default upper range.
        \endlist

        \e{Note: Only in the form of JSON object array, it can be used in Qt Quick Designer}
    */
    property var model

    implicitWidth: 300
    implicitHeight: 200
    background:  ColorMap.monitors_background
    foreground: ColorMap.foreground

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
        legendVisible: root.legendVisible
        property var xAxis: Axis {
            type: Axis.Bottom
            label: xLabel || "Time (%1)".arg(getTimeLabel(root.units))
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
        onContextMenuRequested: contextMenu.popup()
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: 'Axes Range'
            onTriggered: {
                dialog.pvs = d.pvs
                dialog.open()
            }
        }
        MenuItem {
            text: 'Legend'
            checkable: true
            checked: plot.legendVisible
            onTriggered: plot.legendVisible = checked
        }
    }

    ListModel {
        id: sourceModel
        ListElement {
            text: 'Channel'
            value: LimitsSource.Channel
        }
        ListElement {
            text: 'Default'
            value: LimitsSource.Default
        }
        ListElement {
            text: 'User'
            value: LimitsSource.User
        }
    }

    Dialog {
        id: dialog
        title: 'Define Axis Range'

        property var pvs

        contentItem: GridLayout {
            anchors.fill: parent
            columns: 5
            rowSpacing: 2
            anchors.margins: 5

            Repeater {
                model: dialog.pvs
                Text {
                    text: modelData.source
                    horizontalAlignment: Text.AlignRight
                    Layout.row: index
                    Layout.column: 0
                }
            }
            Repeater {
                model: dialog.pvs
                ComboBox {
                    model: sourceModel
                    textRole: "text"
                    currentIndex: modelData.limits.loprSrc
                    onCurrentIndexChanged: {
                        modelData.limits.loprSrc = sourceModel.get(currentIndex).value
                    }
                    Layout.row: index
                    Layout.column: 1
                }
            }
            Repeater {
                model: dialog.pvs
                TextField {
                    implicitWidth: 80
                    text: modelData.limits.lopr
                    validator: DoubleValidator {}
                    enabled: modelData.limits.loprSrc == LimitsSource.User
                    onAccepted: modelData.limits.loprUser = parseFloat(text)
                    Layout.row: index
                    Layout.column: 2
                }
            }
            Repeater {
                model: dialog.pvs
                ComboBox {
                    model: sourceModel
                    textRole: "text"
                    currentIndex: modelData.limits.hoprSrc
                    onCurrentIndexChanged: {
                        modelData.limits.hoprSrc = sourceModel.get(currentIndex).value
                    }
                    Layout.row: index
                    Layout.column: 3
                }
            }
            Repeater {
                model: dialog.pvs
                TextField {
                    implicitWidth: 80
                    text: modelData.limits.hopr
                    validator: DoubleValidator {}
                    enabled: modelData.limits.hoprSrc == LimitsSource.User
                    onAccepted: modelData.limits.hoprUser = parseFloat(text)
                    Layout.row: index
                    Layout.column: 4
                }
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

    Component.onCompleted: createTraces()

    /*! \internal */
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
            graph.name = pen.label || pen.channel
            graph.lineStyle = PlotStyle.Line
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
              cmd +='        loprChannel: range.lower\n' +
                    '        hoprChannel: range.upper\n' +
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
               return "sec";
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
            d.graphs[i].y.rescale()
        }
        plot.replot()
    }
    /*! \internal */
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
