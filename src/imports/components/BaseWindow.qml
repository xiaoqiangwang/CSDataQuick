import QtQml 2.0
import QtQuick 2.0
import QtQuick.Window 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Private 1.0

/*!
    \qmltype BaseWindow
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.bases
    \brief Base for all toplevel windows.

    In addition to \l Window, it records the file path this window represents and the macro expansion.
    Also it implements a context menu,
    \list
    \li Print - capture a screenshot of the current window and send it to printer
    \li Close - close the current window
    \li Main Window - popup the main window if any registered
    \li Display List - popup the window to display windows opened
    \li Data Engines - popup the window to display data engine information
    \endlist

    \e {Note: This is actually a Rectangle type, because Window derived types cannot be used in Qt Quick Designer.}
*/
Rectangle {
    id: root
    /*! This property holds the window title */
    property string title

    implicitWidth: 640
    implicitHeight: 480

    color: ColorMap.windows_background

    onWidthChanged: {
        if (Utils.inPuppet)
            return

        if (d.oldWidth !== NaN)
            Utils.resizeChildItems(root, width / d.oldWidth, 1)

        d.oldWidth = width
    }

    onHeightChanged: {
        if (Utils.inPuppet)
            return

        if (d.oldHeight !== NaN)
            Utils.resizeChildItems(root, 1, height / d.oldHeight)

        d.oldHeight = height
    }

    QtObject {
        id: d
        property double oldWidth: NaN
        property double oldHeight: NaN
    }

    ContextMenu {
        anchors.fill: parent
    }
}
