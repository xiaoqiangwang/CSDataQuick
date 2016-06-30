pragma Singleton
import QtQuick 2.0
import QtQuick.Dialogs 1.2

Dialog {
    id: dialog
    title: 'PV Info'

    property string info

    contentItem: Text {
        text: info
    }
}
