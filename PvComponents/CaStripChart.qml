import QtQuick 2.0

import PvComponents 1.0

Item {
    id: control

    property color foreground
    property color background

    property var _pvs: []
    property var _graphs: []

    property var _time: []
    property var _data: []

    property int numPoints: 1000
    property ListModel models: ListModel {
        ListElement {
            property Limits limits
            channel: 'catest'
            foreground: 'red'
        }
    }

    Plot {
        id: plot
        anchors.fill: parent
    }

    Component.onCompleted: {
        for(var i=0; i<models.count; i++) {
            if (models.get(i).channel == '')
                continue
            var graph = plot.addGraph()
            var pv = Qt.createQmlObject('import PvComponents 1.0; PvObject{channel: "%1"}'.arg(models.get(i).channel), control, 'pv'+i)
            _pvs.push(pv)
            _graphs.push(graph)
        }
        plot.replot()
    }
    Timer {
        running: true
        repeat: true
        interval: 1000
        onTriggered: {
            var d = new Date()
            var date = d.valueOf();
            console.log(date)
            for(var i=0; i<_pvs.length; i++) {
                if (_time.length > numPoints) {
                    _time.pop()
                    _data.pop()
                }
                _time.push(date)
                _data.push(Qt.point(date, pv.value))
                _graphs[i].data = _data
            }
            plot.replot()
        }
    }
}
