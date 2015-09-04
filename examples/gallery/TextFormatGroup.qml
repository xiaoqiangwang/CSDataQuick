import QtQuick 2.0
import QtQuick.Controls 1.0

import PvComponents 1.0

GroupBox {
    property int textFormat: textFormatGroup.current.textFormatOption

    title: "Text Format"

    Column {
        ExclusiveGroup { id: textFormatGroup }
        RadioButton {
            property int textFormatOption: TextFormat.Decimal
            text: "Decimal"
            checked: true
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.Exponential
            text: "Exponential"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.EngNotation
            text: "Engineering Notation"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.Compact
            text: "Compact"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.Truncated
            text: "Trancated"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.Hexadecimal
            text: "Hexadecimal"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.Octal
            text: "Octal"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.String
            text: "String"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.Sexagesimal
            text: "Sexagesimal"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.SexagesimalDMS
            text: "SexagesimalDMS"
            exclusiveGroup: textFormatGroup
        }
        RadioButton {
            property int textFormatOption: TextFormat.SexagesimalHMS
            text: "SexagesimalHMS"
            exclusiveGroup: textFormatGroup
        }
    }
}

