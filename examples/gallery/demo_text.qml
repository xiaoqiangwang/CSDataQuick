import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    Column {
        anchors.fill: parent
        CaText {
            text: input.text
            width: 200
            height: textHeight.value
        }
        TextField {
            id: input
            width: 200
            text: 'enter text here'
        }
        Row {
            Text {
                text: 'Text Height'
            }
            Slider {
                id: textHeight
                minimumValue: 8
                maximumValue: 50
            }
        }
    }
}
