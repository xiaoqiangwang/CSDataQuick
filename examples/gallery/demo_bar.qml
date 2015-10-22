import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0


Rectangle {
ColumnLayout {
    anchors.fill: parent
    CaBar {
        width: (direction == Direction.Left || direction == Direction.Right) ? 100 : 50
        height: (direction == Direction.Left || direction == Direction.Right) ? 50 : 100
        source: 'catest'
        foreground: fgColorPalette.color
        background: bgColorPalette.color
        colorMode: colorModeGroup.colorMode
        direction: directionGroup.direction
        labelStyle: labelStyleGroup.labelStyle
        fillMode: fillModeGroup.fillMode
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
    ColorModeGroup {
        id: colorModeGroup
    }
    DirectionGroup {
        id: directionGroup
    }
    LabelStyleGroup {
        id: labelStyleGroup
    }
    FillModeGroup {
        id: fillModeGroup
    }
}
}
