import QtQml 2.0
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
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

    ComboBox {
        id: combo_control
        anchors.fill: parent
        enabled: csdata.accessRight & CSData.WriteAccess
        model: Utils.inPuppet ? [source] : csdata.stateStrings
        onModelChanged: {
            if (csdata.value !== undefined)
                currentIndex = csdata.value
        }

        style: ComboBoxStyle {
            id: cbStyle
            padding { top: 2 ; left: 2 ; right: 14 ; bottom:2 }
            label: Item {
                implicitWidth: textitem.implicitWidth
                baselineOffset: textitem.y + textitem.baselineOffset
                Text {
                    id: textitem
                    text: Utils.inPuppet ? root.source : combo_control.currentText
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                    color: (colorMode == ColorMode.Alarm && !Utils.inPuppet) ? root.alarmColor : root.foreground
                    font.pixelSize: root.font.size
                    font.family: root.font.family
                }
            }
            background: StyledFrame {
                implicitWidth: Math.round(TextSingleton.implicitHeight * 4.5) + 10
                implicitHeight: Math.max(25, Math.round(TextSingleton.implicitHeight * 1.2)) 
                StyledFrame {
                    width: 9
                    height: 7
                    shadow: FrameShadow.Raise
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 7
                    opacity: control.enabled ? 0.7 : 0.5
                    color: root.background
                }
                shadow: FrameShadow.Raise
                color: root.background
            }
            panel: Item {
                property bool popup: false
                property font font: cbStyle.font
                property color textColor: cbStyle.textColor
                property color selectionColor: cbStyle.selectionColor
                property color selectedTextColor: cbStyle.selectedTextColor
                property int dropDownButtonWidth: cbStyle.dropDownButtonWidth
                anchors.centerIn: parent
                anchors.fill: parent
                implicitWidth: labelLoader.implicitWidth + 20
                implicitHeight: Math.max(labelLoader.implicitHeight + padding.top + padding.bottom, backgroundLoader.implicitHeight)
                baselineOffset: labelLoader.item ? padding.top + labelLoader.item.baselineOffset: 0
                Loader {
                    id: backgroundLoader
                    anchors.fill: parent
                    sourceComponent: background
                }

                Loader {
                    id: labelLoader
                    sourceComponent: label
                    anchors.fill: parent
                    anchors.leftMargin: padding.left
                    anchors.topMargin: padding.top
                    anchors.rightMargin: padding.right
                    anchors.bottomMargin: padding.bottom
                }
            }
        }
/*
        delegate: ItemDelegate {
            background: root.background
            text: modelData
        }

        indicator: StyledFrame {
            width: 9
            height: 7
            shadow: FrameShadow.Raise
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 7
            opacity: control.enabled ? 0.7 : 0.5
            color: root.background
        }

        contentItem: Text {
            text: combo_control.currentText
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
            font.pixelSize: root.font.size
            font.family: root.font.family
        }
        background: StyledFrame {
            anchors.fill: parent
            shadow: FrameShadow.Raise
            color: root.background
        }
*/
        onActivated: {
            csdata.setValue(index)
        }
    }
    Connections {
        target: csdata
        onValueChanged: {
            combo_control.currentIndex = csdata.value
        }
    }
}
