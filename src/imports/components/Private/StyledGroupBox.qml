import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Compat 1.0 as Compat

Item {
    id: root

    property alias title: label.text
    property alias font: label.font

    onWidthChanged: {
        if (Utils.inPuppet)
            return

        if (!Number.isNaN(d.oldWidth)) {
            Utils.resizeChildItems(root, width / d.oldWidth, 1)
        }
        d.oldWidth = width
    }

    onHeightChanged: {
        if (Utils.inPuppet)
            return

        if (!Number.isNaN(d.oldHeight)) {
            Utils.resizeChildItems(root, 1, height / d.oldHeight)
        }
        d.oldHeight = height
    }

    Compat.Label {
        id: label
        anchors.top: parent.top
    }

    BorderImage {
        id: frame
        anchors.fill: parent
        anchors.topMargin: padding / 2
        source: "images/groupbox.png"
        border.left: 4
        border.right: 4
        border.top: 4
        border.bottom: 4
    }

    /*! \internal */
    QtObject {
        id: d
        property bool componentCompleted: false
        property var rootItem: null
        property double oldWidth: NaN
        property double oldHeight: NaN
    }


}
