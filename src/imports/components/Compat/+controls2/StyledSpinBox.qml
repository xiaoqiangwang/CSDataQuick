import QtQuick 2.0
import QtQuick.Controls 2.3

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0 as Private

SpinBox {
    id: root

    property double doubleValue: 0
    property string suffixText: ''
    property color backgroundColor: 'white'
    property color foregroundColor: 'black'
    property double minimumValue: 0
    property double maximumValue: 99
    property int decimals: 0
    value: doubleValue * d.divider
    from: minimumValue * d.divider
    to: maximumValue * d.divider
    editable: true

    implicitHeight: 25

    spacing: 2

    leftPadding: spacing
    rightInset: up.implicitIndicatorWidth + spacing
    rightPadding:  rightInset

    textFromValue: function(value) {
        return Utils.convert(TextFormat.String, value / d.divider, decimals) + suffixText
    }

    valueFromText: function(text) {
        if (suffixText) {
            var n = text.lastIndexOf(suffixText)
            if (n >= 0)
                text = text.substring(0, n)
        }
        return Utils.parse(TextFormat.Decimal, text) * d.divider
    }

    validator: DoubleValidator {
        top: root.maximumValue
        bottom: root.minimumValue
        decimals: root.decimals
    }

    onValueModified: {
        doubleValue = value / d.divider
    }

    background: Private.StyledFrame {
        color: root.backgroundColor
        shadow: FrameShadow.Sunken
    }

    contentItem: TextInput {
        clip: true
        font: root.font
        color: root.foregroundColor
        text: root.displayText
        readOnly: !root.editable
        validator: root.validator
        verticalAlignment: TextInput.AlignVCenter
    }

    up.indicator: Text {
        anchors.bottom: parent.verticalCenter
        x: parent.width - width
        color: root.up.pressed ? Qt.darker(root.backgroundColor) : root.backgroundColor
        text: "\u25B2"
        font: root.font
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignBottom
    }

    down.indicator: Text {
        id: downIndicator
        anchors.bottom: parent.verticalCenter
        x: parent.width - width
        color: root.down.pressed ? Qt.darker(root.backgroundColor) : root.backgroundColor
        text: "\u25B2"
        font: root.font
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignBottom
        transform: Scale { yScale: -1; origin.y: downIndicator.height }
    }

    QtObject {
        id: d
        property double divider: Math.pow(10, decimals)
    }
}
