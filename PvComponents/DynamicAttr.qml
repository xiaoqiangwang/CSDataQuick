import QtQuick 2.1

import PvComponents 1.0

/*!
    \qmltype DynamicAttr
    \inqmlmodule PvComponents
    \brief Specifies items visibility.

    Fours process variables can be speficied for the calculation.
*/

QtObject {
    id: da
    /*! Name of the main process variable */
    property string channel
    /*! Name of the second process variable */
    property string channelB
    /*! Name of the third process variable */
    property string channelC
    /*! Name of the forth process variable */
    property string channelD

    /*!
        \qmlproperty enumeration visibilityMode
    */
    property int visibilityMode : VisibilityMode.Static
    /*!
        An expression that determines whether the object is displayed or not.
        The expression should return 0 for false and anything else for true.
    */
    property string visibilityCalc : 'A'
    /*! The visibility as determend from visiblityMode */
    property bool visibility: true

    /*! \internal */
    property var chanList: new Array(4)
    /*! \internal */
    property var _arg: [0,0,0,0,0,0,0,0,0,0,0,0]

    /*! This signal is emitted when the severity of the main process variable changes. */
    signal statusChanged

    /*! \internal */
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
    }

    /*! \internal */
    function updateStatus() {
        calcPerform()
        statusChanged()
    }

    /*! \internal */
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
