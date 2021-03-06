import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0


ColumnLayout {
    CSByte {
        width: orientation == Qt.Horizontal ? 300 : 30
        height: orientation == Qt.Vertical ? 300 : 30
        foreground: fgColorPalette.color
        background: bgColorPalette.color
        source: 'calc'
        start: start.value
        end: end.value
        orientation: orientationGroup.orientation
    }
    Row {
        spacing: 10
        Text {
            text: 'foreground'
        }

        ColorPalette {
            id: fgColorPalette
            width: 100
            height: 20
            currentIndex: 14
        }
    }
    Row {
        spacing: 10
        Text {
            text: 'background'
        }

        ColorPalette {
            id: bgColorPalette
            width: 100
            height: 20
            currentIndex: 4
        }
    }
    OrientationGroup {
        id: orientationGroup
    }
    Row {
        Text {
            text: 'Start Bit'
        }
        Slider {
            id: start
            value: 0
            minimumValue: 0
            maximumValue: 31
            stepSize: 1
        }
    }
    Row {
        Text {
            text: 'End Bit'
        }
        Slider {
            id: end
            value: 15
            minimumValue: 0
            maximumValue: 31
            stepSize: 1
         }
    }
}
