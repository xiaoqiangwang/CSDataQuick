import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import PvComponents 1.0

/*!
    \qmltype CaByte
    \inqmlmodule PvComponents
    \brief Display bits of an unsigned integer.

    It display values in binary.  It masks off the lower 32 bits and treats them as an unsigned integer.
    The bit set is filled with foreground color, and the unset bit is filled with background color.

    \qml
    CaByte {
        channel: 'calc'
    }
    \endqml

    \image byte.png
*/
CaMonitor {
    id: root
    /*!
      \qmlproperty enumeration orientation
      \list
        \li Qt.Horizontal: Start Bit at the left and End Bit at the right
        \li Qt.Vertical: Start Bit at the top and End Bit at the bottom
      \endlist
    */
    property int orientation: Qt.Horizontal
    /*!
        The integer is in the range 0-31, specifying the starting bit to be displayed at the left or top.
    */

    property int start: 15
    /*!
        The integer is in the range 0-31, specifying the ending bit to be displayed at the right or bottom.
        The ending bit can be smaller than starting bit, which causes the direction to appear reversed.
    */
    property int end: 0

    Component {
        id: horz
        Row {
            Repeater {
                id: rptr
                model: Math.abs(end - start) + 1
                Rectangle{
                    width: loader.width / rptr.count
                    height: loader.height
                    border.width: 1
                }
            }
        }
    }

    Component {
        id: vert
        Column {
            Repeater {
                id: rptr
                model: Math.abs(end - start) + 1
                Rectangle {
                    width: loader.width
                    height: loader.height / rptr.count
                    border.width: 1
                }
            }
        }
    }
    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: orientation == Qt.Horizontal ? horz : vert
    }

    Connections {
        target: pv
        onValueChanged: {
            var sign = start < end ? 1 : -1;
            for (var i=0; i<Math.abs(end - start) + 1 && i<loader.item.children.length; i++)
                loader.item.children[i].color = (pv.value & Math.pow(2, start+i*sign))
                        ? (colorMode == ColorMode.Static ? foreground : alarmColor)
                        : background
        }
    }
}
