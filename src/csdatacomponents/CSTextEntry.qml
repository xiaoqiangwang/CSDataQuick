import QtQuick 2.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSTextEntry
    \inqmlmodule CSDataQuick.Components
    \brief Display an editable text

    The Text Entry provides a means of displaying the value of a process variable in an entry box,
    where it can be edited and changed. The value is not sent until Return is pressed with the focus in the entry box.
    If the focus leaves the entry box before Return is pressed, the value of the process variable is not changed
    and the current value is restored into the entry box.  The resulting value of the process variable is shown
    when and only when the cursor leaves the entry box.
    The value after leaving the entry box can be used to verify that the input was accepted.

    If the native format of the process variable associated with the Text Entry is DBR_STRING or DBR_CHAR,
    then the string entered in the Text Entry will be sent to the process variable.

    If the native type is DBR_ENUM, then it will first try to match the entered string to
    one of the state strings of the process variable.  If that fails, it will try to
    interpret the string as a number representing one of the state strings and send that number to the process variable.

    For the native types for which a number is meaningful, the behavior of the Text Entry will depend on the format of the Text Entry.
    If the Text Entry has a format of octal, then numbers may be input with or without a leading 0 and will be interpreted as octal.
    That is, entering either 010 or 10 results in 8 decimal.  Entering 8 is an error.
    The numbers will be displayed with a leading 0 when they are updated.
    Similarly, if the Text Entry is formatted to hexadecimal,
    then numbers may be input with or without a leading 0x or 0X and will be interpreted as hexadecimal.
    In this case, 0x10, 010 or 10 will result in 16 decimal.
    In all other formats, input values will be interpreted as decimal unless they begin with 0x or 0X,
    in which case they are interpreted as hexadecimal.
    Input numbers with a leading 0 in these formats will not be interpreted as octal because of the chance for error or confusion.
    If you want to input octal, you must format the Text Entry to octal.

    \qml
    Column {
        CSTextEntry {
            width: 80
            height: 25
            source: 'bo'
        }
        CSTextEntry {
            width: 200
            height: 25
            source: 'wavec'
        }
        CSTextEntry {
            width: 80
            height: 25
            source: 'catest'
            format: TextFormat.Decimal
        }
        CSTextEntry {
            width: 80
            height: 25
            source: 'catest'
            format: TextFormat.Hexadecimal
        }
        CSTextEntry {
            width: 80
            height: 25
            source: 'catest'
            format: TextFormat.Octal
        }
    }
    \endqml

    \image textentry.png
*/

CSControl {
    id: root
    implicitWidth: 100
    implicitHeight: 20
    /*!
        \qmlproperty enumeration align
        Sets the horizontal alignment of the text within the item width.

        The font used depends on the item height using function \l UtilsJS::getBestFontSize.
    */
    property alias align: textEntry.horizontalAlignment
    /*!
        \qmlproperty TextFormat format
    */
    property int format: TextFormat.Decimal
    /*! \internal */
    property var font: UtilsJS.getBestFontSize(height, true)

    limits: Limits {
        precChannel: pv.precision
    }

    StyledTextEntry {
        id: textEntry
        anchors.fill: parent
        foreground: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
        background: root.background
        font.pixelSize: root.font.size
        font.family: root.font.family
        horizontalAlignment: root.align
        readOnly: pv.accessRight & CSData.WriteAccess == 0

        onHasFocusChanged: {
            text = formatString(format, pv.value)
        }

        onAccepted: {
            if (pv.accessRight & CSData.WriteAccess == 0)
                return
            var value
            switch (pv.fieldType) {
            case CSData.String:
                value = text
                break
            case CSData.Enum:
                var i
                for(i=0; i<pv.stateStrings.length; i++)
                    if (text == pv.stateStrings[i])
                        break
                if (i < pv.stateStrings.length)
                    value = i
                else
                    value = Utils.parse(format, text)
                break
            case CSData.Char:
                if (format == TextFormat.String)
                    value = text
                else
                    value = Utils.parse(format, text)
                break
            default:
                value = Utils.parse(format, text)
            }
            if (value !== undefined && (typeof value == 'string' || !isNaN(value))) {
                pv.value = value
            }
        }
    }

    Connections {
        target: pv

        onSourceChanged: {
            textEntry.text = formatString(format, pv.value)
        }

        onStateStringsChanged: {
            textEntry.text = formatString(format, pv.value)
        }

        onValueChanged: {
            textEntry.text = formatString(format, pv.value)
            if (!textEntry.hasFocus)
                textEntry.cursorPosition = 0
        }
    }

    Connections {
        target: limits

        onPrecChanged: {
            textEntry.text = formatString(format, pv.value)
        }
    }

    onFormatChanged: {
        textEntry.text = formatString(format, pv.value)
    }

    /*!
        \internal
        Format the value based on PV type.
    */
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
