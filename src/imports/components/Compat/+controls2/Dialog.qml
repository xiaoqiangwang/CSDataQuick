import QtQuick 2.0
import QtQuick.Controls 2.2

Dialog {
    property bool withCancel: false
    
    implicitWidth: implicitContentWidth + padding * 2 + margins * 2

    standardButtons: Dialog.Ok | (withCancel ? Dialog.Cancel : 0)
}
