import QtQuick 2.6
import QtQuick.Layouts 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSTextUpdate
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.monitors
    \brief Display the value in given format.

    The value is format to string using \l format.

    The \l font property can specify the font family, pixelSize, weight, style etc. If unspecified font family is returned from
    function \l UtilsJS::getBestFontSize.

    If \l fontSizeMode is Text.Fit, the font pixelSize will fit within the item width and height. If \l fontSizeMode is Text.FixedSize,
    Text.VerticalFit or Text.HorizontalFit, the text may extend beyond the height and width specified for the Text item
    or may not use up all the specified height and width. The text typically does not fill all of the specified height for the Text item.

    \qml
    Row {
        spacing: 5
        CSTextUpdate {
            width: 100
            height: 20
            source: 'catest.SCAN'
        }
        CSTextUpdate {
            width: 100
            height: 20
            source: 'catest'
        }
    }
    \endqml

    \image textupdate.png
*/

CSMonitor {
    id: root
    /*!
        \qmlproperty enumeration align
        Set the horizontal alignment of the text within the item width.

        The valid values for horizontalAlignment are Text.AlignLeft, Text.AlignRight, Text.AlignHCenter and Text.AlignJustify.
    */
    property alias align: label_control.horizontalAlignment

    /*!
        \qmlproperty font font
        The text font.
    */
    property alias font: label_control.font
    /*!
        \qmlproperty enumeration fontSizeMode
        This property specifies how the font size of the displayed text is determined.

        \sa {Text::} {fontSizeMode}
    */
    property alias fontSizeMode: label_control.fontSizeMode
    /*!
        This property indicates how the data value is formated to text display. \sa TextFormat,

        For all of the formats, the result depends on the number and the precision in limits.
    */
    property int format: TextFormat.Decimal
    /*! This property indicates whether to display the physical units if available */
    property bool unitsVisible: false

    implicitWidth: label_control.implicitWidth
    implicitHeight: fontSizeMode == Text.FixedSize ? label_control.implicitHeight : 16

    limits.precChannel: csdata.precision

    TextFormatter {
        id: formatter
        data: root.csdata
        format: root.format
        precision: root.limits.prec
    }

    Text {
        id: label_control
        anchors.fill: parent
        fontSizeMode: Text.Fit
        minimumPixelSize: 6
        font.family: UtilsJS.getBestFontSize(root.height).family
        Binding on font.pixelSize {
            when: root.fontSizeMode != Text.FixedSize && !Utils.inPuppet
            value: root.height
        }
        verticalAlignment: Text.AlignVCenter
        text: Utils.inPuppet ? root.source : (formatter.text + (root.unitsVisible ? ' ' + root.csdata.units : ''))
        color: (root.colorMode == ColorMode.Alarm && !Utils.inPuppet) ? root.alarmColor : root.foreground
        clip: true
    }
}

