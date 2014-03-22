import QtQuick 2.0

import PvComponents 1.0

Item {
    id: control

    property color foreground: 'black'
    property color background:  '#bbbbbb'
    property int count: 10
    property string countPvName: 'calc'

    property var _pvCount

    // list of created pvs and their corresponding graph and time,data
    property var _pvs: []
    property var _graphs: []
    property var _data: []

    property ListModel models: ListModel {
        ListElement {
            xchannel: 'catest'
            foreground: 'red'
            property Limits limits
        }
        ListElement {
            xchannel: 'calc'
            foreground: 'blue'
            property Limits limits
        }
    }

    Plot {
        id: plot
        anchors.fill: parent
    }

    Timer {
        repeat: true
        interval: 1000
        running: true
        onTriggered: {}
    }

    Component.onCompleted: {
        if (countPvName != '') {
            _pvCount = Qt.createQmlObject('import PvComponents 1.0; PvObject{channel: "%1"}'.arg(countPvName), control, 'pvCount')
            _pvCount.valueChanged.connect(updateCount)
        }

        for(var i=0; i<models.count; i++) {
            var xchannel = models.get(i).xchannel
            var ychannel = models.get(i).ychannel

            _data.push([])
            var graph = plot.addGraph()
            graph.color = models.get(i).foreground

            var xpv = null, ypv = null
            if (xchannel && xchannel != '') {
                xpv = Qt.createQmlObject('import PvComponents 1.0; PvObject{channel: "%1"}'.arg(xchannel), control, 'xpv'+i)
                xpv.valueChanged.connect(updateData)
                xpv.connectionChanged.connect(updateConnection)
            }
            if (ychannel && ychannel != '') {
                ypv = Qt.createQmlObject('import PvComponents 1.0; PvObject{channel: "%1"}'.arg(ychannel), control, 'ypv'+i)
                ypv.valueChanged.connect(updateData)
                ypv.connectionChanged.connect(updateConnection)
            }
            _pvs.push([xpv,ypv])
            _graphs.push(graph)
            pv.valueChanged.connect(update);
        }
        plot.replot()

    }

    function updateCount() {
        count = _pvCount.value
    }

    function updateConnection() {

    }

    function updateData() {
        for (var i=0; i<_pvs.length; i++) {
            var xpv = _pvs[i][0]
            var ypv = _pvs[i][1]

            if (xpv == null) {
                xdata = new Array(count)
                if (!ypv.connected)
                    continue
                if (ypv.count == 1)
                    ydata.append(ypv.value)
            }
        }
    }
}
