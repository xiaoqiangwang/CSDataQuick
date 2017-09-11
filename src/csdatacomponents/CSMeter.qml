import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSMeter
    \inqmlmodule CSDataQuick.Components
    \brief Displays the value on a cirular dial panel.

    The value of the process variable is displayed by a needle within an arc.
    \qml
    CSMeter {
        source: 'catest'
        labelStyle: LabelStyle.Outline
    }
    \endqml

    \image meter.png
*/

CSMonitor {
    id: root

    /*!
        \qmlproperty enumeration labelStyle
        The decoration mode.

        \list
        \li LabelStyle.Frame - No extra features.
        \li LabelStyle.None - Same as LabelStyle.Frame.
        \li LabelStyle.Outline - Show the limits.
        \li LabelStyle.Limits - Show limits and a box for the value.
        \li LabelStyle.Channel - In addition to LabelStyle.Limits, show the process variable name.
        \endlist
    */
    property int labelStyle: LabelStyle.Frame
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
    readonly property var font: UtilsJS.getBestFontSize(height / 8, 0)

    implicitWidth: 150
    implicitHeight: 50

    /*! The operation limits */
    limits: Limits {
        id: limits
        precChannel: pv.precision
    }

    Meter {
        id: meter
        anchors.fill: parent

        background: root.background
        indicatorColor: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground

        minimumValue: limits.lopr
        maximumValue: limits.hopr
        precision: limits.prec

        font.family: root.font.family
        font.pixelSize: root.font.size

        title: root.source
        showTitle: labelStyle == LabelStyle.Channel
        showRange: true
        showValue: labelStyle == LabelStyle.Limits || labelStyle == LabelStyle.Channel
    }

    Connections {
        target: pv
        onRangeChanged: {
            if (pv.range.isValid()) {
                limits.loprChannel = pv.range.lower
                limits.hoprChannel = pv.range.upper
            }
        }
        onValueChanged: {
            meter.value = Number(pv.value).toFixed(limits.prec)
        }
    }
}
