/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "enums.h"

/*!
    \qmltype TextFormat
    \inqmlmodule CSDataQuick.Components
    \brief Enums of supported text format

    This class holds the enums of the supported text format. \sa Utils::convert
*/

/*!
    \qmlproperty enumeration TextFormat::TextFormat

    \table
    \row
        \li TextFormat.Decimal
        \li The text is a number with or without a decimal point,
            but not in exponential notation. E.g.10000.00 or 10000.
            Very large numbers, which do not fit in the allocated space for text,
            are converted to exponential regardless.
            It is what is returned by the EPICS routine cvtDoubleToString().
    \row
        \li TextFormat.Exponential
        \li The text is in exponential notation.  E.g. 1.00e+04.
    \row
        \li TextFormat.EngNotation
        \li The text is in engineering notation. E.g. 10.00e+03.
    \row
        \li TextFormat.Compact
        \li The text is supposedly in the most compact form of decimal or exponential.
            (Similar to the %g format in C.)  The form may not be the most compact,
            however.  It is what is returned by the EPICS routine cvtDoubleToCompactString().
    \row
        \li TextFormat.Truncated
        \li The text is truncated to the largest integer. E.g. 10000.
    \row
        \li TextFormat.Hexadecimal
        \li The text is truncated to the nearest integer and shown in hexadecimal.
            E.g. 0x3e8.  Input is interpreted as hexadecimal.
    \row
        \li TextFormat.Octal
        \li The text is truncated to the nearest integer and shown in octal. E.g. 01750.
            Input is interpreted as octal.
   \row
        \li TextFormat.String
        \li This is the same as decimal except that for large numbers or precision,
            it can be in exponential format.
            It is what is returned by the EPICS routine cvtDoubleToString().
    \row
        \li TextFormat.Sexagesiaml
        \li The text is in degrees or hours, minutes, and seconds with colons between the three fields.
            E.g. 12:45:10.2. The degrees or hours field is the integer part of the number,
            and the fractional part is expressed as an integer number of minutes and seconds plus fractions of a second.
            The example, 12:45:10.2, is the number 12.752833 expressed as 12 degrees or hours,
            45 minutes, and 10.2 seconds. The precision determines the number of digits after the first colon.
            For precisions of 0, 1, 2, 3, 4, 5, and 6 the number above would be expressed as 13, 12:5, 12:45, 12:45:1, 12:45:10, 12:45:10.2, and 12:45:10.20, respectively.
            On input colons may be replaced by spaces or omitted, and a decimal point may be used.
            The inputs 10.75, 10:45, 10.45.0, and 10 45 00 are all treated as a quarter to eleven.
            The inputs ‑1, :‑1, and ::‑1 are interpreted as ‑1:00:00, ‑:01:00, and ‑0:00:01, respectively.
    \row
        \li TextFormat.SexagesimalDMS
        \li This is the same as sexagesimal except that the number is assumed to be in radians
            and is expressed as degrees, minutes, and seconds with 2p radians representing 360 degrees.
            On input the number is multiplied by 180/p and then converted to sexagesimal as above.
            On output the number is converted as sexagesimal above and then multiplied by p/180 to get radians.
    \row
        \li TextFormat.SexagesimalHMS
        \li This is the same as sexagesimal except that the number is assumed to be in radians
            and is expressed as hours, minutes, and seconds with 2p radians representing 24 hours.
            On input the number is multiplied by 12/p and then converted to sexagesimal as above.
            On output the number is converted as sexagesimal above and then multiplied by p/12 to get radians.
    \endtable
*/


/*!
    \qmltype ColorMode
    \inqmlmodule CSDataQuick.Components
    \brief Enums of dynamic color rules

    This class holds the enums of the dynamic color rules.
*/

/*!
    \qmlproperty enumeration ColorMode::ColorMode

    \table
    \row
        \li ColorMode.Static
        \li Show the object in its normal colors.
    \row
        \li ColorMode.Alarm
        \li Show the object in alarm colors based on the severity of the associated process variable.
            The alarm colors are Green for NO_ALARM, Yellow for MINOR_ALARM, Red for MAJOR_ALARM, White for INVALID_ALARM,
            and Gray if the alarm has an unknown value (because of an error in the record, for example).
    \row
        \li ColorMode.Discrete
        \li Same as ColorMode.Static
    \endtable
*/

/*!
    \qmltype Direction
    \inqmlmodule CSDataQuick.Components
    \brief Enums of item direction
*/
/*!
    \qmlproperty enumeration Direction::Direction
    \list
        \li Direction.Up
        \li Direction.Right
        \li Direction.Down
        \li Direction.Left
    \endlist
*/
/*!
    \qmltype EdgeStyle
    \inqmlmodule CSDataQuick.Components
    \brief Enums of the outline style
*/
/*!
    \qmlproperty enumeration EdgeStyle::EdgeStyle
    \list
        \li EdgeStyle.Solid
        \li EdgeStyle.Dash
    \endlist
*/

/*!
    \qmltype FillStyle
    \inqmlmodule CSDataQuick.Components
    \brief Enums of the item filling style
*/
/*!
    \qmlproperty enumeration FillStyle::FillStyle
    \list
        \li FillStyle.Solid
        \li FillStyle.Outline
    \endlist
*/

/*!
    \qmltype VisibilityMode
    \inqmlmodule CSDataQuick.Components
    \brief Enums of how items visibility changes
*/
/*!
    \qmlproperty enumeration VisibilityMode::VisibilityMode
    \table
    \row
        \li VisibilityMode.Static
        \li The item is displayed always.
    \row
        \li VisibilityMode.IfNotZero
        \li The object is displayed if the process variable is not zero.
    \row
        \li VisibilityMode.IfZero
        \li The object is displayed if the object is zero.
    \row
        \li VisibilityMode.Calc
        \li Use calc expression to determine the visability.
    \endtable
*/
/*!
    \qmltype LimitsSource
    \inqmlmodule CSDataQuick.Components
    \brief Enums of where the limits and precision is from
*/
/*!
    \qmlproperty enumeration LimitsSource::LimitsSource
    \list
        \li LimitsSource.Channel
        \li LimitsSource.Default
        \li LimitsSource.User
        \li LimitsSource.Unused
    \endlist
*/

/*!
    \qmltype ArrowPosition
    \inqmlmodule CSDataQuick.Components
    \brief Enums of where the arrow is placed
*/

/*!
    \qmlproperty enumeration ArrowPosition::ArrowPosition
    \list
        \li ArrowPosition.None
        \li ArrowPosition.Start
        \li ArrowPosition.End
        \li ArrowPosition.Both
    \endlist
*/
