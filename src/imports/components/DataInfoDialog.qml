pragma Singleton
import QtQuick 2.0
import CSDataQuick.Components.Compat 1.0 as Compat

Compat.Dialog {
    id: root

    property string info

    title: 'Data Info'

    Compat.Label {
        anchors.fill: parent
        text: root.info
    }
}
