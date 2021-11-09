import QtQml 2.0
import QtQuick 2.5

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Compat 1.0 as Compat
import "utils.js" as UtilsJS

/*!
    \qmltype CSSpinBox
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.controls
    \brief Display a spin box control.

    It allows the user to choose a value by clicking the up or down buttons, or by pressing up or down on the keyboard.
    The user can also type the value in manually.

    The font used depends on the item height using function \l UtilsJS::getBestFontSize.
*/

CSControl {
    id: root
    implicitWidth: spin.implicitWidth
    implicitHeight: spin.implicitHeight

    /*! This property indicates whether to display the physical units if available */
    property bool unitsVisible: false
    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height, 1)

    limits.precChannel: csdata.precision

    QtObject {
        id: d
        // SpinBox emits valueChanged signal during creation and this value
        // should not be updated to CSData. So set the initial value to true.
        property bool blockUpdate: true
    }

    Compat.StyledSpinBox {
        id: spin
        anchors.fill: parent
        font.pixelSize: root.font.size
        font.family: root.font.family
        suffixText: root.unitsVisible ? csdata.units : ''
        decimals: limits.prec
        minimumValue: limits.lopr
        maximumValue: limits.hopr

        foregroundColor: (colorMode == ColorMode.Alarm && alarmMode == AlarmMode.Foreground && !Utils.inPuppet) ? root.alarmColor : root.foreground
        backgroundColor: (colorMode == ColorMode.Alarm && alarmMode == AlarmMode.Background && !Utils.inPuppet) ? root.alarmColor : root.background

        onDoubleValueChanged: if (!d.blockUpdate) csdata.value = doubleValue
    }

    Connections {
        target: csdata
        onRangeChanged: {
            if (csdata.range.isValid()) {
                limits.loprChannel = csdata.range.lower
                limits.hoprChannel = csdata.range.upper
            }
        }
        onValueChanged: {
            d.blockUpdate = true
            spin.doubleValue = csdata.value
            d.blockUpdate = false
        }
    }
}
