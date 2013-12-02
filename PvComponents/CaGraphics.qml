import QtQuick 2.0

import PvComponents 1.0

Item {
    // graphics basic attributes
    property color foreground: 'black'
    property color background: 'transparent'
    property int fill: FillStyle.Outline
    property int edge: EdgeStyle.Solid
    property int fontSize: 12
    property string fontFamily: fontSize <= 22 ? 'Dejavu Sans Mono' : 'Helvetica'

    property int lineWidth: 1
    property int colorMode: ColorMode.Static

    // dynamic attributes
    property alias visibilityMode: da.visibilityMode
    property alias visibilityCalc: da.visibilityCalc
    property alias channel: da.channel
    property alias channelB: da.channelB
    property alias channelC: da.channelC
    property alias channelD: da.channelD
    visible: da.visibility
    property var da: DynamicAttr { id: da }

    Connections {
        target: da

        onStatusChanged: {
            if (colorMode == ColorMode.Static)
                return
            switch (da.chanList[0].severity) {
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
