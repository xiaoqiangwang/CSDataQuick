import QtQuick 2.1

import PvComponents 1.0


QtObject {
    id: da
    property string channel
    property string channelB
    property string channelC
    property string channelD

    property int visibilityMode : VisibilityMode.Static
    property string visibilityCalc : 'A'
    property bool visibility: true

    property var chanList: new Array(4)
    property var _arg: [0,0,0,0,0,0,0,0,0,0,0,0]

    signal statusChanged
    signal update

    function calcPerform() {
        if (chanList[0] != null) {
            _arg[0] = chanList[0].value
            _arg[6] = chanList[0].count
            _arg[7] = chanList[0].updisplim
            _arg[8] = chanList[0].status
            _arg[9] = chanList[0].severity
            _arg[10] = chanList[0].prec
            _arg[11] = chanList[0].lodisplim
        }
        if (chanList[1] != null) {
            _arg[1] = chanList[1].value
        }
        if (chanList[2] != null) {
            _arg[2] = chanList[2].value
        }
        if (chanList[3] != null) {
            _arg[3] = chanList[3].value
        }
        if (visibilityMode == VisibilityMode.Calc) {
            var res = Utils.calculate(visibilityCalc, _arg)
            visibility = (res != 0)
        } else if (visibilityMode == VisibilityMode.IfZero)
            visibility = (_arg[0] == 0)
        else if (visibilityMode == VisibilityMode.IfNotZero)
            visibility = (_arg[0] != 0)
        else
            visibility = true
        update()
    }

    function updateStatus() {
        calcPerform()
        statusChanged()
    }

    function createChannels () {
        var chan
        if (channel != '') {
            chan = Qt.createQmlObject('import PvComponents 1.0; PvObject {channel: "%1"}'.arg(channel), da, 'channel')
            chan.valueChanged.connect(calcPerform)
            chan.statusChanged.connect(updateStatus)
            da.chanList[0] = chan

        }
        if (channelB != '') {
            chan = Qt.createQmlObject('import PvComponents 1.0; PvObject {channel: "%1"}'.arg(channelB), da, 'channelB')
            chan.valueChanged.connect(calcPerform)
            chanList[1] = chan
        }
        if (channelC != '') {
            chan = Qt.createQmlObject('import PvComponents 1.0; PvObject {channel: "%1"}'.arg(channelC), da, 'channelC')
            chan.valueChanged.connect(calcPerform)
            chanList[2] = chan
        }
        if (channelD != '') {
            chan = Qt.createQmlObject('import PvComponents 1.0; PvObject {channel: "%1"}'.arg(channelD), da, 'channelD')
            chan.valueChanged.connect(calcPerform)
            chanList[3] = chan
        }
    }

    Component.onCompleted: {
        createChannels()
    }
}
