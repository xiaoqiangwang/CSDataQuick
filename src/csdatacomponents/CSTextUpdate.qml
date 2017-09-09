import QtQuick 2.0
import QtQuick.Layouts 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSTextUpdate
    \inqmlmodule CSDataQuick.Components
    \brief Display the value in given format.

    The value is format to string using \l format.

    The font used depends on the item height using function \l UtilsJS::getBestFontSize.
    If \l align is not Text.AlignLeft, it will attemp to find a font size so that the text can fit in item width.

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
        This property indicates how the data value is formated to text display. \sa TextFormat,

        For all of the formats, the result depends on the number and the precision in limits.
    */
    property int format: TextFormat.Decimal
    /*! This property indicates whether to display the physical units if available */
    property bool unitsVisible: false
    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height)

    implicitWidth: 100
    implicitHeight: 20

    limits: Limits {
        precChannel: pv.precision
    }

    RowLayout {
        anchors.fill: parent
        Text {
            id: label_control
            //text: formatString(format, pv.value)
            color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
            clip: true
            font.pixelSize: root.font.size
            font.family: root.font.family
            Layout.fillWidth: true
        }
        Text {
            id: units
            font: label_control.font
            color: root.foreground
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            visible: unitsVisible && text != ''
            text: pv.units
        }
    }

    Connections {
        target: pv

        onSourceChanged: {
            label_control.text = formatString(format, pv.value)
        }

        onStateStringsChanged: {
            label_control.text = formatString(format, pv.value)
        }

        onValueChanged: {
            label_control.text = formatString(format, pv.value)
            // MEDM Compat: automatic adjust font size only if it is not left aligned
            if (align == Text.AlignLeft)
                return
            while(label_control.font.pixelSize > 6 && label_control.paintedWidth > label_control.width) {
                label_control.font.pixelSize -= 1
            }
        }
    }

    Connections {
        target: limits

        onPrecChanged: {
            label_control.text = formatString(format, pv.value)
        }
    }

    onFormatChanged: {
        label_control.text = formatString(format, pv.value)
    }

    /*! \internal */
    function formatString () {
        if (pv.extraProperties['QmlPuppetMode'])
            return pv.source

        return UtilsJS.formatString(pv.value,
                                    format,
                                    pv.fieldType,
                                    limits.prec,
                                    pv.stateStrings)
    }
}

