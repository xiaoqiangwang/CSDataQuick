import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaTextEntry {
            channel: 'catest'
            width: 100
            height: 20
            foreground: fgColorPalette.color
            background: bgColorPalette.color
            align: textAlignmentGroup.textAlignment
        }
        CaTextEntry {
            channel: 'castr'
            width: 100
            height: 20
            foreground: fgColorPalette.color
            background: bgColorPalette.color
            align: textAlignmentGroup.textAlignment
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
        TextAlignmentGroup {
            id: textAlignmentGroup
        }
    }
}
