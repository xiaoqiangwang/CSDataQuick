import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CaComposite
    \inqmlmodule CSData.Components
    \brief Display grouped items or include external files.

    A Composite is a group of any items. These items can be created directly.
    Or it can be made to get its items from an external ADL or QML file specified by \l source.

    The Composite will ignore things like the display and colormap in the ADL file and just use the items in it.
    The positions of the items will be adjusted so the upper left corner of the bounding box of all of them is
    at the specified x and y values of the Composite.
    The specified height and width of the Composite are adjusted to those of the bounding box.

    If the file has macros, you can specify them by \l macro. If you do not specify any,
    any existing macros in the current file which uses this Composite will be passed to the file.
    If you use this feature, those macros will not be passed,
    and only the ones you specify with \l macro will be used instead.

    \qml
    BaseWindow {
        CaComposite {
            source: 'controls.adl'
        }
    }
    \endqml
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

    // Mask when PVs disconnected
    Rectangle {
        z: 1
        anchors.fill: parent
        color: 'white'
        visible: !da.connected
    }

    Component.onCompleted: {
        if (!source)
            return
        var absFilePath = Utils.searchADLFile(source, baseWindow.filePath ? baseWindow.filePath : baseWindow.url)
        if (!absFilePath) {
            console.error("Failed to find file ", source)
            return
        }
        var qmlBody = Utils.openADLComposite(absFilePath, macro)
        var qmlCmd = 'import QtQuick 2.0\n' +
                'import QtQuick.Controls 1.0\n' +
                'import CSData.Components 1.0\n' +
                'Item { anchors.fill: parent\n' +
                qmlBody + '\n}';
        var item = Qt.createQmlObject(qmlCmd, root, absFilePath)
    }
}