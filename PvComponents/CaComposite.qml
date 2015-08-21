import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaComposite
    \inqmlmodule PvComponents
    \brief Display grouped items

*/

Item {
    // appearance
    /*! background color */
    property color background: ColorMap.color51 // color4 is the actual default
    /*! foreground color */
    property color foreground: ColorMap.color14
    /*!
        \qmlproperty enumeration colorMode

        It controls wether the item color changes according to alarm severity
        of the associated process variable.
    */
    property int colorMode: ColorMode.Static

    /*! dynamic attributes */
    property DynamicAttr dynamicAttr: DynamicAttr { id: da }

    visible: da.visibility

    Connections {
        target: da

        onStatusChanged: {
            if (colorMode == ColorMode.Static)
                return
            switch (da.chanList[0].severity) {
                case 0: // NO_ALARM
                foreground = 'green'
                break;
                case 1: // MINOR_ALARM
                foreground = 'yellow'
                break;
                case 2: // MAJOR_ALARM
                foreground = 'red'
                break;
                case 3: // INVALID_ALARM
                foreground = 'white'
                break;
            }
        }
    }
}
