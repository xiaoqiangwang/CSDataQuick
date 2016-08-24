import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0
import "utils.js" as UtilsJS

/*!
    \qmltype CSTextEntryArray
    \inqmlmodule CSDataQuick.Components
    \brief Display text entry for array data

    \qml
    Column {
        CSTextEntryArray {
            source: 'x'
            count: 5
        }
        CSTextEntryArray {
            source: 'y'
            count: 5
            stacking: Stacking.Row
        }
    }
    \endqml

    \image textentryarray.png
*/

CSControl {
    id: root
    implicitWidth: stacking == Stacking.Column ? 150 : count * 100
    implicitHeight: stacking == Stacking.Column ? count * 20 : 40
    /*!
      Set the layout for the text entries. Default is Stacking.Column.

      \list
        \li Stacking.Row: The entries are arranged in a row
        \li Stacking.Column: The entries are arranged in a column
      \endlist
    */
    property int stacking: Stacking.Column
    /*!
        \qmlproperty enumeration align
        Sets the horizontal alignment of the text within the item width.

        The font used depends on the item height using function \l UtilsJS::getBestFontSize.
    */
    property int align: Text.AlignLeft
    /*!
        \qmlproperty TextFormat format
    */
    property int format: TextFormat.Decimal
    /*!
        \qmlproperty int index
        Sets the start index
    */
    property int index: 0
    /*!
        \qmlproperty int count
        Sets the number of elements
    */
    property int count: 1
    /*!
        Show the element label
    */
    property bool showLabel: true
    /*!
        Element label
    */
    property var label: []
    /*! \internal */
    property var font: UtilsJS.getBestFontSize(stacking==Stacking.Column ? height / count: height / 2, true)

    limits: Limits {
        precChannel: pv.precision
    }

    Component {
        id: textEntry
        StyledTextEntry {
            foreground: colorMode == ColorMode.Alarm ? root.alarmColor : root.foreground
            background: root.background
            font.pixelSize: root.font.size
            font.family: root.font.family
            horizontalAlignment: root.align
            readOnly: pv.accessRight & CSData.WriteAccess == 0
            Layout.fillWidth: stacking == Stacking.Column
            Layout.fillHeight: stacking != Stacking.Column
            Layout.row: stacking == Stacking.Column ? index : 1
            Layout.column: stacking == Stacking.Column ? 1 : index

            onHasFocusChanged: formatString()

            onAccepted: {
                if (pv.accessRight & CSData.WriteAccess == 0)
                    return
                var value = text
                if (pv.fieldType == CSData.Char && format == TextFormat.String) {
                    // if char is formated as string, convert string back into char code
                    value = text.charCodeAt(0)
                } else if (pv.fieldType !== CSData.String) {
                    // if data type is numeric, parse the string input based on format
                    value = Utils.parse(format, text)
                }
                if (typeof value == 'string' || !isNaN(value)) {
                    // convert data vector into array and change the modified element
                    // the conversion is necessary because QML only recognise
                    // QVector<double> and QVector<int>
                    var array = pv.value
                    array[index + root.index] = value
                    pv.value = array
                }
            }
        }
    }

    Component {
        id: textLabel
        Label {
            text: index < label.length ? label[index] : '#%1'.arg((index + root.index).toString())
            font.pixelSize: root.font.size
            font.family: root.font.family
            Layout.fillWidth: stacking == Stacking.Column
            Layout.fillHeight: stacking != Stacking.Column
            Layout.row: stacking == Stacking.Column ? index : 0
            Layout.column: stacking == Stacking.Column ? 0 : index
            visible: showLabel
        }
    }

    GridLayout {
        id: layout
        anchors.fill: parent
        rows: stacking == Stacking.Column ? count : 2
        columns: stacking == Stacking.Column ? 2 : count
        rowSpacing: 0
        columnSpacing: 0
        Repeater {
            id: labels
            model: root.count
            delegate: textLabel
        }
        Repeater {
            id: entries
            model: root.count
            delegate: textEntry
        }
    }

   Connections {
        target: pv
        onPrecisionChanged: formatString()
        onValueChanged: formatString()
   }

   Connections {
       target: limits
       onPrecChanged: formatString()
   }

   onFormatChanged: formatString()

   function formatString () {
       if (pv.value === undefined)
           return
       for(var i=0; i<count; i++) {
           entries.itemAt(i).text = UtilsJS.formatString(pv.value[i],
                                                         format,
                                                         pv.fieldType,
                                                         limits.prec,
                                                         pv.stateStrings)
       }
   }
}
