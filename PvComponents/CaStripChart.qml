import QtQuick 2.0

import PvComponents 1.0

Item {
    id: control

    property string title
    property color foreground: 'black'
    property color background:  '#bbbbbb'

    // list of created pvs and their corresponding graph and time,data
    property var _pvs: []
    property var _graphs: []
    property var _time: []
    property var _data: []

    property int maxSeconds: 60
    property ListModel models

    Plot {
        id: plot
        anchors.fill: parent
        title: control.title
        foreground: control.foreground
        background: control.background
        Axis {
            id: xAxis
            type: Axis.Bottom
            scale: Axis.DateTime
        }
        Axis {
            id: yAxis
            type: Axis.Left
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

        Repeater {
            model: control.models
            Graph {
                //x: modelData.x
                //y: modelData.y
            }
        }
    }

    Component.onCompleted: {
        /*
        for(var i=0; i<models.count; i++) {
            if (models.get(i).channel == '')
                continue
            _time.push([])
            _data.push([])

            graph.color = models.get(i).foreground
            var pv = Qt.createQmlObject('import PvComponents 1.0; PvObject{channel: "%1"}'.arg(models.get(i).channel), control, 'pv'+i)
            _pvs.push(pv)
            _graphs.push(graph)
            pv.valueChanged.connect(update);
        }
        plot.replot()
        */
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

    function update() {
        var d = new Date()
        var date = d.valueOf() / 1000;
        // check buffer
        for(var i=0; i<_pvs.length; i++) {
            if (_time[i][_time[i].length-1] - _time[i][0] > maxSeconds) {
                _time[i].shift()
                _data[i].shift()
            }
            _time[i].push(date)
            _data[i].push(Qt.point(date, _pvs[i].value))
            _graphs[i].data = _data[i]
            //_graphs[i].addData(date, _pvs[i].value)
        }
        plot.replot()
    }
}
