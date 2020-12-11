import QtQml 2.0
import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSMeter
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.monitors
    \brief Displays the value on a cirular dial panel.

    The value of the dta is displayed by a needle within an arc.
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
        \li LabelStyle.Channel - In addition to LabelStyle.Limits, show the data source name.
        \endlist
    */
    property int labelStyle: LabelStyle.Frame
    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height / 8, 0)

    implicitWidth: 150
    implicitHeight: 50

    /*! The operation limits */
    limits.precChannel: csdata.precision

    Meter {
        id: meter
        anchors.fill: parent

        background: root.background
        indicatorColor: (colorMode == ColorMode.Alarm && !Utils.inPuppet) ? root.alarmColor : root.foreground

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
        target: csdata
        onRangeChanged: {
            if (csdata.range.isValid()) {
                limits.loprChannel = csdata.range.lower
                limits.hoprChannel = csdata.range.upper
            }
        }
        onValueChanged: {
            meter.value = Number(csdata.value).toFixed(limits.prec)
        }
    }
}
