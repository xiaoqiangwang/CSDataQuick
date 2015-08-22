import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import PvComponents 1.0

/*!
    \qmltype CaBar
    \inqmlmodule PvComponents
    \brief Display a bar that expands or contracts when value changes

*/

CaMonitor {
    id: root
    /*!
        \qmlproperty enumeration label
        The decoration mode.
    */
    property int label: LabelStyle.Frame
    /*!
        \qmlproperty enumeration direction
        The expanding direction.
    */
    property int direction: Direction.Right
    /*! Operation limits range and precision */
    property Limits limits : Limits{}
}
