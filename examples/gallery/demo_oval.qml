import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CSOval {
            id: oval
            width: 200
            height: 200
            foreground: fgColorPalette.color
            fillStyle: fillStyleGroup.fillStyle
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
        FillStyleGroup {
            id: fillStyleGroup
        }
    }
}
