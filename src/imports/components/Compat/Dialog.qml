import QtQuick 2.0
import QtQuick.Dialogs 1.2

Dialog {
    property bool modal: false
    property bool withCancel: false

    modality: modal ? Qt.WindowModal : Qt.NonModal
    standardButtons: StandardButton.Ok | (withCancel ? StandardButton.Cancel : 0)
}
