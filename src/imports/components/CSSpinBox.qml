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
    \brief Display a spin box control

    It allows the user to choose a value by clicking the up or down buttons, or by pressing up or down on the keyboard.
    The user can also type the value in manually.

    The font used depends on the item height using function \l UtilsJS::getBestFontSize.
*/

CSControl {
    id: root
    implicitWidth: 100
    implicitHeight: 20

    /*! This property indicates whether to display the physical units if available */
    property bool unitsVisible: false
    /*! \internal */
    property alias limits_loprSrc: limits.loprSrc
    /*! \internal */
    property alias limits_loprDefault: limits.loprDefault
    /*! \internal */
    property alias limits_hoprSrc: limits.hoprSrc
    /*! \internal */
    property alias limits_hoprDefault: limits.hoprDefault
    /*! \internal */
    property alias limits_precSrc: limits.precSrc
    /*! \internal */
    property alias limits_precDefault: limits.precDefault
    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height, 1)

    limits: Limits {
        id: limits
        precChannel: csdata.precision
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
                color: root.background
            }
        }
        onValueChanged: csdata.value = value        
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
            spin.value = csdata.value 
        }
    }
}
