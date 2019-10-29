import QtQuick 2.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CSComposite
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.graphics
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
        CSComposite {
            source: 'controls.adl'
        }
    }
    \endqml
*/

BaseItem {
    id: root
    /*!
        This property holds the external source file to load components.
    */
    property string source
    /*!
        This property holds the macro substitution for the external source file,
    */
    property string macro

    visible: dynamicAttribute.visibility

    /*! \internal */
    QtObject {
        id: d
        property bool componentCompleted: false
        property var rootItem: null
        property double oldWidth: NaN
        property double oldHeight: NaN
    }

    // Mask when PVs disconnected
    Rectangle {
        z: 1
        anchors.fill: parent
        color: 'white'
        visible: !dynamicAttribute.connected
    }

    onMacroChanged: {
        createDisplay()
    }

    onSourceChanged: {
        createDisplay()
    }

    Component.onCompleted: {
        d.componentCompleted = true
        createDisplay()
    }

    onWidthChanged: {
        if (Utils.inPuppet)
            return

        if (d.oldWidth !== NaN) {
            if (d.rootItem) {
                d.rootItem.width = width
                Utils.resizeChildItems(d.rootItem, width / d.oldWidth, 1)
            } else
                Utils.resizeChildItems(root, width / d.oldWidth, 1)
        }
        d.oldWidth = width
    }

    onHeightChanged: {
        if (Utils.inPuppet)
            return

        if (d.oldHeight !== NaN) {
            if (d.rootItem) {
                d.rootItem.height = height
                Utils.resizeChildItems(d.rootItem, 1, height / d.oldHeight)
            } else
                Utils.resizeChildItems(root, 1, height / d.oldHeight)
        }
        d.oldHeight = height
    }

    /*! \internal */
    function createDisplay () {
        // postpone until component completed
        if (!d.componentCompleted)
            return

        if (d.rootItem) {
            d.rootItem.destroy()
            d.rootItem = null;
        }

        if (!source)
            return

        var absFilePath = Utils.searchDisplayFile(source, windowPath)

        if (absFilePath.toString() === '') {
            console.error("Failed to find file ", source)
            return
        }

        d.rootItem = Utils.createComponentByFile(root, absFilePath, join(windowMacro, macro))

        // override rootItem position property
        if (d.rootItem) {
            d.rootItem.x = 0
            d.rootItem.y = 0
            if (d.rootItem.width)
                implicitWidth = d.rootItem.width
            if (d.rootItem.height)
                implicitHeight = d.rootItem.height
        }
    }

    /*! \internal */
    function join(s1, s2) {
        if (!s1)
            return s2
        if (!s2)
            return s1
        return s1 + "," + s2
    }
}
