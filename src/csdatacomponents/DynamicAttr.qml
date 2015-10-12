import QtQuick 2.1

import CSDataQuick.Components 1.0

/*!
    \qmltype DynamicAttr
    \inqmlmodule CSDataQuick.Components
    \brief Specifies items visibility.

    Fours process variables can be speficied for the calculation.
*/

/*!
    \qmlproperty string channel
    Name of the main process variable
*/
/*!
    \qmlproperty string channelB
    Name of the second process variable
*/
/*!
    \qmlproperty string channelC
    Name of the third process variable
*/
/*!
    \qmlproperty string channelD
    Name of the forth process variable
*/
/*!
    \qmlproperty enumeration visibilityMode
    \list
    \li VisibilityMode.Static - No visiblity change.
    \li VisibilityMode.IfZero - Visible if \l channel has value zero.
    \li VisibilityMode.IfNotZero - Visible if \l channel has value no-zero.
    \li VisibilityMode.Calc - Visible if \l visibilityCalc returns true.
    \endlist
*/
/*!
    \qmlproperty string visibilityCalc
    An expression that determines whether the object is displayed or not.
    The expression should return 0 for false and anything else for true.
*/

DynamicAttributeBase {
    id: da
    /*! The color as determined from channel A's severity */
    property color alarmColor: ColorMap.no_alarm
    /*! The visibility as determend from visiblityMode */
    property bool visibility: true

    /*! This property indicates wether channels are connect */
    readonly property bool connected:
        (!d.pvA || d.pvA.connected) &&
        (!d.pvB || d.pvB.connected) &&
        (!d.pvC || d.pvC.connected) &&
        (!d.pvD || d.pvD.connected)

    /*! \internal */
    property QtObject d: QtObject {
        id: d
        property var pvA
        property var pvB
        property var pvC
        property var pvD
        /*! \internal */
        property var args: [0,0,0,0,0,0,0,0,0,0,0,0]
    }

    /*! This signal is emitted when the severity of the main process variable changes. */
    signal statusChanged

    /*! This signal is emitted when the expression gets evaluated. */
    signal update

    /*! \internal */
    function calcPerform() {
        if (d.pvA) {
            d.args[0] = d.pvA.value
            d.args[6] = d.pvA.count
            d.args[7] = d.pvA.range.upper
            d.args[8] = d.pvA.alarm.status
            d.args[9] = d.pvA.alarm.severity
            d.args[10] = d.pvA.precicion
            d.args[11] = d.pvA.range.lower
        }
        if (d.pvB) {
            d.args[1] = d.pvB.value
        }
        if (d.pvC) {
            d.args[2] = d.pvC.value
        }
        if (d.pvD) {
            d.args[3] = d.pvD.value
        }
        if (visibilityMode == VisibilityMode.Calc) {
            var res = Utils.calculate(visibilityCalc, d.args)
            visibility = (res != 0)
        } else if (visibilityMode == VisibilityMode.IfZero)
            visibility = (d.args[0] == 0)
        else if (visibilityMode == VisibilityMode.IfNotZero)
            visibility = (d.args[0] != 0)
        else
            visibility = true
        update();
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
            chan = Qt.createQmlObject('import CSDataQuick.Data 1.0; CSData {source: "%1"}'.arg(channel), da, 'channel')
            chan.valueChanged.connect(calcPerform)
            chan.alarmChanged.connect(updateStatus)
            d.pvA = chan

        }
        if (channelB != '') {
            chan = Qt.createQmlObject('import CSDataQuick.Data 1.0; CSData {source: "%1"}'.arg(channelB), da, 'channelB')
            chan.valueChanged.connect(calcPerform)
            d.pvB = chan
        }
        if (channelC != '') {
            chan = Qt.createQmlObject('import CSDataQuick.Data 1.0; CSData {source: "%1"}'.arg(channelC), da, 'channelC')
            chan.valueChanged.connect(calcPerform)
            d.pvC = chan
        }
        if (channelD != '') {
            chan = Qt.createQmlObject('import CSDataQuick.Data 1.0; CSData {source: "%1"}'.arg(channelD), da, 'channelD')
            chan.valueChanged.connect(calcPerform)
            d.pvD = chan
        }
    }

    Component.onCompleted: {
        createChannels()
    }
}
