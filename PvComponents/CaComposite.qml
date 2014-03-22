import QtQuick 2.0

import PvComponents 1.0

Item {
    // appearance
    property color background: '#73dfff' // '#bbbbbb' is the actual default
    property color foreground: '#000000'
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
