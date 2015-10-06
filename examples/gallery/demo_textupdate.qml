import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaTextUpdate {
            width: 100
            height: 20
            source: 'catest'
            foreground: fgColorPalette.color
            background: bgColorPalette.color
            colorMode: colorModeGroup.colorMode
            align: textAlignmentGroup.textAlignment
            format: textFormatGroup.textFormat
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
        TextAlignmentGroup {
            id: textAlignmentGroup
        }
        TextFormatGroup {
            id: textFormatGroup
        }
    }
}
