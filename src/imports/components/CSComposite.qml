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
    /*! \internal */
    property alias dynamicAttribute_visibilityMode: da.visibilityMode
    /*! \internal */
    property alias dynamicAttribute_visibilityCalc: da.visibilityCalc
    /*! \internal */
    property alias dynamicAttribute_channel: da.channel
    /*! \internal */
    property alias dynamicAttribute_channelB: da.channelB
    /*! \internal */
    property alias dynamicAttribute_channelC: da.channelC
    /*! \internal */
    property alias dynamicAttribute_channelD: da.channelD

    implicitWidth: 400
    implicitHeight: 300
    dynamicAttribute: DynamicAttribute { id: da }
    visible: da.visibility

    /*! \internal */
    QtObject {
        id: d
        property var rootItem: null
    }

    // delay createDisplay until attached to a window instance
    Timer {
        id: delay
        interval: 50
        onTriggered: createDisplay()
    }

    // Mask when PVs disconnected
    Rectangle {
        z: 1
        anchors.fill: parent
        color: 'white'
        visible: !da.connected
    }

    onMacroChanged: {
        createDisplay()
    }

    onSourceChanged: {
        createDisplay()
    }

    Component.onCompleted: {
        delay.start()
    }

    /*! \internal */
    function createDisplay () {
        if (d.rootItem) {
            d.rootItem.destroy()
            d.rootItem = null;
        }

        if (!source)
            return

        var absFilePath = Utils.searchDisplayFile(source, baseWindow)
        if (absFilePath == '') {
            console.error("Failed to find file ", source)
            return
        }
        var windowMacro = Utils.getProperty(baseWindow, "macro")

        var qmlCmd
        if (/.adl$/i.test(absFilePath)) {
            var qmlBody = Utils.openADLComposite(absFilePath, macro ? macro : windowMacro)
            qmlCmd = 'import QtQuick 2.0\n' +
                    'import QtQuick.Controls 1.0\n' +
                    'import CSDataQuick.Components 1.0\n' +
                    'Item { anchors.fill: parent\n' +
                    qmlBody + '\n}';
        } else if (/.edl$/i.test(absFilePath)) {
            var qmlBody = Utils.openEDLComposite(absFilePath, join(windowMacro, macro))
            qmlCmd = 'import QtQuick 2.0\n' +
                    'import QtQuick.Controls 1.0\n' +
                    'import CSDataQuick.Data 1.0\n' +
                    'import CSDataQuick.Components 1.0\n' +
                    'Item { anchors.fill: parent\n' +
                    qmlBody + '\n}';
        } else if (/.ui$/i.test(absFilePath)) {
            var qmlBody = Utils.openUIComposite(absFilePath, join(windowMacro, macro))
            qmlCmd = 'import QtQuick 2.0\n' +
                    'import QtQuick.Controls 1.0\n' +
                    'import CSDataQuick.Data 1.0\n' +
                    'import CSDataQuick.Components 1.0\n' +
                    'Item { anchors.fill: parent\n' +
                    qmlBody + '\n}';
         } else if (/.qml$/i.test(absFilePath)) {
            qmlCmd = Utils.openQMLDisplay(absFilePath, macro ? macro : windowMacro)
        }
        d.rootItem  = Qt.createQmlObject(qmlCmd, root, absFilePath)
        // override rootItem position property
        d.rootItem.x = 0
        d.rootItem.y = 0
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
