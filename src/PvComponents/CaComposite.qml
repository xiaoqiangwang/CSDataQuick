import QtQuick 2.0

import PvComponents 1.0

/*!
    \qmltype CaComposite
    \inqmlmodule PvComponents
    \brief Display grouped items

*/

BaseItem {
    id: root

    // appearance
    /*! background color */
    property color background: ColorMap.color51 // color4 is the actual default
    /*! foreground color */
    property color foreground: ColorMap.color14

    /*!
        external source file to load components
    */
    property string source
    /*!
        macro substitution for external source file
    */
    property string macro

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

    Component.onCompleted: {
        if (!source)
            return
        var absFilePath = Utils.searchADLFile(source, baseWindow.filePath)
        if (!absFilePath) {
            console.error("Failed to find file ", source)
            return
        }
        var qmlBody = Utils.openADLComposite(absFilePath, macro)
        var qmlCmd = 'import QtQuick 2.0\n' +
                'import QtQuick.Controls 1.0\n' +
                'import PvComponents 1.0\n' +
                'Item { anchors.fill: parent\n' +
                qmlBody + '\n}';
        var item = Qt.createQmlObject(qmlCmd, root, absFilePath)
    }
}
