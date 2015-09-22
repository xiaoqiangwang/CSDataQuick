import QtQuick 2.0

import PvComponents 1.0


/*!
    \qmltype CaGraphics
    \inqmlmodule PvComponents
    \brief Basic type to all PV graphics shape

    CaGraphics is the base type for all graphics types.
    It defines all the attributes that are common across graphics shapes.

*/
Item {
    // graphics basic attributes
    /*! foreground color */
    property color foreground: ColorMap.color14
    /*! background color */
    property color background: 'transparent'
    /*! Draw outline only or fill the shape */
    property int fill: FillStyle.Solid
    /*! Outline style */
    property int edge: EdgeStyle.Solid
    /*! Outline width */
    property int lineWidth: 0
    /*!
        \qmlproperty enumeration colorMode
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
            switch (da.d.pvA.severity) {
                case 0: // NO_ALARM
                foreground = ColorMap.no_alarm
                break;
                case 1: // MINOR_ALARM
                foreground = ColorMap.minor_alarm
                break;
                case 2: // MAJOR_ALARM
                foreground = ColorMap.major_alarm
                break;
                case 3: // INVALID_ALARM
                foreground = ColorMap.invalid_alarm
                break;
            }
        }
        onConnectedChanged: {
            if (colorMode == ColorMode.Static && da.visibilityMode == VisibilityMode.Static)
                return
            if (!da.connected)
                foreground = ColorMap.invalid_alarm
        }
    }
}
