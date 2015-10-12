import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype Limits
    \inqmlmodule CSDataQuick.Components
    \brief  Data range and precision

    It manages the data range and precision from different sources.
*/
QtObject {
    /*!
        The source of lower operation range.
    */
    property int  loprSrc: LimitsSource.Channel
    /*!
        The lower operation range from control data source.
    */
    property real loprChannel: 0.0
    /*!
        The lower operation range given by display designer.
    */
    property real loprDefault: 0.0
    /*!
        The lower operation range given by user.
    */
    property real loprUser: 0.0
    /*!
        The lower operation range.
    */
    property real lopr: loprSrc == LimitsSource.Channel ? loprChannel : (loprSrc == LimitsSource.Default ? loprDefault : loprUser)

    /*!
        The source of upper operation range.
    */
    property int  hoprSrc: LimitsSource.Channel
    /*!
        The upper operation range from control data source.
    */
    property real hoprChannel: 1.0
    /*!
        The upper operation range given by display designer.
    */
    property real hoprDefault: 1.0
    /*!
        The upper operation range given by user.
    */
    property real hoprUser: 1.0
    /*!
        The upper operation range.
    */
    property real hopr: hoprSrc == LimitsSource.Channel ? hoprChannel : (hoprSrc == LimitsSource.Default ? hoprDefault : hoprUser)

    /*!
        The source of data precision.
    */
    property int  precSrc: LimitsSource.Channel
    /*!
        The data precision from control data source.
    */
    property int  precChannel: 0
    /*!
        The data precision given by display designer.
    */
    property int  precDefault: 0
    /*!
        The data precision given by user.
    */
    property int  precUser: 0
    /*!
        The data precision.
    */
    property int  prec: precSrc == LimitsSource.Channel ? precChannel : (precSrc == LimitsSource.Default ? precDefault : precUser)
}
