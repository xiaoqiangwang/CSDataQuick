import QtQuick 2.0
import QtQuick.Controls 1.0
import PvLibrary 1.0

Label {
    property alias channel: pv.channel
    property bool alarmColor: false
    property bool withUnits: false

    text: pv.value + withUnits ? ' ' + pv.units : ''

    PvObject {
        id: pv
        asstring: true
    }

    Connections {
        target: pv
        onStatusChanged: {
            if (alarmColor) {
                switch (pv.severity) {
                case 0: // NO_ALARM
                    color = 'green'
                    break;
                case 1: // MINOR_ALARM
                    color = 'yellow'
                    break;
                case 2: // MAJOR_ALARM
                    color = 'red'
                    break;
                case 3: // INVALID_ALARM
                    color = 'white'
                    break;
                }
            }
        }
    }
}
