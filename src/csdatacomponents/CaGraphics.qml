import QtQuick 2.0

import CSDataQuick.Components 1.0


/*!
    \qmltype CaGraphics
    \inqmlmodule CSDataQuick.Components
    \brief Basic type to all PV graphics shape

    CaGraphics is the base type for all graphics types.
    It defines all the attributes that are common across graphics shapes.

*/
BaseItem {
    // graphics basic attributes
    /*! 
        \qmlproperty color foreground
        color to draw text, lines and fill shapes
    */
    foreground: ColorMap.color14
    /*! 
        \qmlproperty color background
        not used for graphics items
    */
    background: 'transparent'
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
    property alias dynamicAttr: da
    dynamicAttribute: DynamicAttr {id: da}

    /*! color based on the severity of the process variable */
    property color alarmColor: ColorMap.invalid_alarm

    visible: da.visibility

    Connections {
        target: da
        onStatusChanged: {
            if (colorMode == ColorMode.Static)
                return
            switch (da.d.pvA.alarm.severity) {
                case 0: // NO_ALARM
                alarmColor = ColorMap.no_alarm
                break;
                case 1: // MINOR_ALARM
                alarmColor = ColorMap.minor_alarm
                break;
                case 2: // MAJOR_ALARM
                alarmColor = ColorMap.major_alarm
                break;
                case 3: // INVALID_ALARM
                alarmColor = ColorMap.invalid_alarm
                break;
            }
        }
        onConnectedChanged: {
            if (colorMode == ColorMode.Static && da.visibilityMode == VisibilityMode.Static)
                return
            if (!da.connected)
                alarmColor = ColorMap.invalid_alarm
        }
    }
}
