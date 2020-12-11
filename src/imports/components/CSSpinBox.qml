import QtQml 2.0
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
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

    SpinBox {
        id: spin
        anchors.fill: parent
        suffix: root.unitsVisible ? csdata.units : ''
        decimals: limits.prec
        stepSize: Math.pow(10, -decimals)
        minimumValue: limits.lopr
        maximumValue: limits.hopr
        style: SpinBoxStyle {
            font.family: root.font.family
            font.pixelSize: root.font.size
            textColor: (colorMode == ColorMode.Alarm && !Utils.inPuppet) ? root.alarmColor : root.foreground
            selectedTextColor: Qt.lighter(textColor)
            selectionColor: Qt.darker(root.background)
            background: Rectangle {
                implicitHeight: Math.max(25, Math.round(styleData.contentHeight * 1.2))
                implicitWidth: styleData.contentWidth + padding.left + padding.right
                baselineOffset: spin.__baselineOffset
                color: root.background
            }
        }
        onValueChanged: if (!d.blockUpdate) csdata.value = value
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
            spin.value = csdata.value
            d.blockUpdate = false
        }
    }
}
