import QtQml 2.0
import QtQuick 2.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Compat 1.0 as Compat
import "utils.js" as UtilsJS

/*!
    \qmltype CSMenu
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.controls
    \brief Displays process variable's enumerate in drop-down menu.

    The Menu is used for ENUM data and is a means for picking the ENUM choices via a menu.
    Note that if the colorMode is alarm, the foreground color, not the background color, is set to the alarm colors.
    The background color should be choosen to contrast with all the alarm colors.

    \qml
    CSMenu {
        source: 'bo'
    }
    \endqml

    \image menu.png
*/

CSControl {
    id: root

    /*! \internal */
    readonly property var font: UtilsJS.getBestFontSize(height - 4, true)

    implicitWidth: combo_control.implicitWidth
    implicitHeight: combo_control.implicitHeight

    Compat.StyledComboBox {
        id: combo_control
        anchors.fill: parent
        backgroundColor: (colorMode == ColorMode.Alarm && alarmMode == AlarmMode.Background && !Utils.inPuppet) ? root.alarmColor : root.background
        foregroundColor: (colorMode == ColorMode.Alarm && alarmMode == AlarmMode.Foreground && !Utils.inPuppet) ? root.alarmColor : root.foreground
        font.pixelSize: root.font.size
        font.family: root.font.family
        enabled: csdata.accessRight & CSData.WriteAccess
        model: Utils.inPuppet ? [source] : csdata.stateStrings
        onModelChanged: {
            if (csdata.value !== undefined)
                currentIndex = csdata.value
        }
        onActivated: {
            csdata.setValue(combo_control.currentIndex)
        }
    }

    Connections {
        target: csdata
        onValueChanged: {
            combo_control.currentIndex = csdata.value
        }
    }
}
