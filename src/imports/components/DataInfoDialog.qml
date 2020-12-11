pragma Singleton
import QtQuick 2.0
import QtQuick.Dialogs 1.2

Dialog {
    id: root
    title: 'Data Info'

    property string info

    contentItem: Text {
        text: root.info
    }
}
