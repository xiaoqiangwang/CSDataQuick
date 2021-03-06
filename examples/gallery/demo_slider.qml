import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0


Rectangle {
ColumnLayout {
    anchors.fill: parent
    CSSlider {
        width:  orientation == Qt.Horizontal ? 150 : 50
        height: orientation == Qt.Vertical ? 150 : 50
        source: 'catest'
        foreground: fgColorPalette.color
        background: bgColorPalette.color
        colorMode: colorModeGroup.colorMode
        direction: directionGroup.direction
        labelStyle: labelStyleGroup.labelStyle
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
            currentIndex: 51
        }
    }
    DirectionGroup {
        id: directionGroup
    }
    LabelStyleGroup {
        id: labelStyleGroup
    }
    ColorModeGroup {
        id: colorModeGroup
    }
}
}
