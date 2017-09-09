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

    Functionally it is the same as \l CSTextEntry, only that it allows to display and change an array data.
    Each array element is displayed in a text entry box, formatted using \l format and \l align.

    The entries can be arranged either into a column or into a row. \l stacking

    An sub-array can be selected using \l index and \l count.

    \l label can be assigned for each array element. By default it is the element index.

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

        The valid values for horizontalAlignment are Text.AlignLeft, Text.AlignRight, Text.AlignHCenter and Text.AlignJustify.
        The font used depends on the item height using function \l UtilsJS::getBestFontSize.
    */
    property int align: Text.AlignLeft
    /*!
        This property indicates how the data value is formated to text display. \sa TextFormat,

        For all of the formats, the result depends on the number and the precision in limits.
    */
    property int format: TextFormat.Decimal
    /*! This property holds the start index of the array data */
    property int index: 0
    /*! This property holds the number of elements starting from \l index */
    property int count: 1
    /*! This property indicates whether to show the element label or not. */
    property bool labelVisible: true
    /*! This property holds the label texts to the elements */
    property var label: []
    /*! \internal */
    property var font: UtilsJS.getBestFontSize(stacking==Stacking.Column ? height / count: height / 2, true)

    implicitWidth: stacking == Stacking.Column ? 150 : count * 100
    implicitHeight: stacking == Stacking.Column ? count * 20 : 40

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
            visible: labelVisible
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

    /*! \internal */
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
