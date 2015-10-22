import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

GroupBox {
    property int labelStyle: labelStyleGroup.current.labelStyleOption

    title: "Label Style"

    Column {
        ExclusiveGroup { id: labelStyleGroup }
        RadioButton {
            property int labelStyleOption: LabelStyle.Frame
            text: "Frame"
            checked: true
            exclusiveGroup: labelStyleGroup
        }
        RadioButton {
            property int labelStyleOption: LabelStyle.None
            text: "No Decoration"
            exclusiveGroup: labelStyleGroup
        }
        RadioButton {
            property int labelStyleOption: LabelStyle.Outline
            text: "Outline"
            exclusiveGroup: labelStyleGroup
        }
        RadioButton {
            property int labelStyleOption: LabelStyle.Limits
            text: "Limits"
            exclusiveGroup: labelStyleGroup
        }
        RadioButton {
            property int labelStyleOption: LabelStyle.Channel
            text: "Channel"
            exclusiveGroup: labelStyleGroup
        }
    }
}

