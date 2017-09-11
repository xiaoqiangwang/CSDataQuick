import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSWheelSwitch
    \inqmlmodule CSDataQuick.Components
    \brief Control a number with individual digit manipulation.

    The WheelSwitch displays a number value. It has arrow buttons above and below the digits that are used to change individual digits.
    The up and down arrow buttons are the main feature of the WheelSwitch. You click them to increment the digit.
    Only the buttons that give allowable values, that is, those between the limits in effect and allowed by the format, are visible at any time.

    The arrow buttons can also be navigated and operated via the keyboard.  The up and down arrow keys increment or decrement the selected digit.
    The left and right arrows keys move the selected digit.
    The selected digit should be highlighted when the pointer is inside the WheelSwitch.
    Clicking on an arrow button moves the selected digit to that digit, in addition to incrementing or decrementing the digit.

    The WheelSwitch will also accept input of a new value via the keyboard.
    When you start typing, the arrow buttons will disappear and what you type will be displayed.
    A minus sign can be typed at any time and toggles having a minus sign at the front of the number.
    Typing a plus sign will delete a minus sign.  Backspace or Delete should delete the last character.
    (This is the only way to correct a typing mistake.)
    The arrow keys cannot be used to move within the input. Escape aborts the input.
    If the pointer leaves the WheelSwitch while you are in input mode, that will also abort the input.
    Type Enter to finish the input. If the number entered is invalid for the format or the limits, it will not be accepted.

    If the number is larger than the format will hold or outside the low and high limits,
    the digits are replaced by astericks, but a minus sign remains if the value is negative.

    The limits and precision can be changed via the PvLimits dialog.

    \qml
    CSWheelSwitch {
        width: 150
        height: 30
        anchors.centerIn: parent
        source: 'catest'
        format: 'PV Value: % 5.2f'
    }
    \endqml

    \image wheelswitch.png
*/

CSControl {
    id: root
    implicitWidth: 100
    implicitHeight: 50
    /*!
        The format used to display the number

        The Format is a string of characters that specifies how the value will be formatted.
        Its syntax is based on the format specification used in the C language, but is more limited.
        Valid formats are of the form “xxx%[space|+]m.nfyyy”.
        They consist of an arbitrary string of characters xxx (the prefix), followed by a % sign, followed by a flag,
        which can be a space or blank, followed by an integer m representing the total number of digits including the sign and the decimal point,
        followed by a decimal point, followed by an integer n representing the number of digits after the decimal point,
        followed by an f, followed by an arbitrary string of characters yyy (the postfix).
        If the flag is +, then the sign is always shown when the value is displayed.
        If the flag is a space, then the sign is shown only when the number is negative.
        The format, apart from the prefix and postfix, is interpreted as it is in the C language,
        but all the options allowed by C are not available.
        The WheelSwitch will check your format and suggest an alternative if it does not like it.
        If the Format is not specified, then the WheelSwitch calculates it based on the low and high limits and the precision.

        \table
        \header
        \li % 6.2f
        \li %+6.2f
        \li Time: % 6.2f sec
        \row
        \li 10.00
        \li +10.00
        \li Time: 10.00 sec
        \row
        \li 99.00
        \li +99.00
        \li Time: 99.00 sec
        \row
        \li -99.00
        \li -99.00
        \li Time: -99.00 sec
        \endtable
    */
    property string format: ''

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
        precChannel: pv.precision
    }

    WheelSwitch {
        id: wheel
        anchors.fill: parent
        lolim: limits.lopr
        hilim: limits.hopr
        prec: limits.prec
        format: root.format
        value: pv.value
        foreground: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
        background: root.background
        onNewValueChanged: pv.value = newValue
    }

    Connections {
        target: pv
        onRangeChanged: {
            if (pv.range.isValid()) {
                limits.loprChannel = pv.range.lower
                limits.hoprChannel = pv.range.upper
            }
        }
    }
}
