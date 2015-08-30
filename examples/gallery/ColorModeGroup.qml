import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

GroupBox {
    property int colorMode: colorModeGroup.current.colorModeOption

    title: "ColorMode"

    Column {
        ExclusiveGroup { id: colorModeGroup }
        RadioButton {
            property int colorModeOption: ColorMode.Static
            text: "Static"
            checked: true
            exclusiveGroup: colorModeGroup
        }
        RadioButton {
            property int colorModeOption: ColorMode.Alarm
            text: "Alarm"
            exclusiveGroup: colorModeGroup
        }
    }
}

