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
    property DynamicAttr dynamicAttr: DynamicAttr { id: da }

    visible: da.visibility

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
