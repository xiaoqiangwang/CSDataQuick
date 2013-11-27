import QtQuick 2.0
import PvComponents 1.0

Item {
    property color background: '#bbbbbb'
    property color foreground: '#000000'
    property string colorMode: 'static'

    // control channel
    property alias channel: pv.channel
    property var pv: PvObject { id: pv; }

    // dynamic attributes
    property alias visibilityMode: da.visibilityMode
    property alias visibilityCalc: da.visibilityCalc
    property alias channelA: da.channel
    property alias channelB: da.channelB
    property alias channelC: da.channelC
    property alias channelD: da.channelD
    visible: da.visibility
    DynamicAttr { id: da }

    Connections {
        target: pv

        onStatusChanged: {
            if (colorMode == 'static')
                return
            switch (pv.severity) {
                case 0: // NO_ALARM
                foreground = 'green'
                break;
                case 1: // MINOR_ALARM
                foreground = 'yellow'
                break;
                case 2: // MAJOR_ALARM
                foreground = 'red'
                break;
                case 3: // INVALID_ALARM
                foreground = 'white'
                break;
            }
        }
    }
}
