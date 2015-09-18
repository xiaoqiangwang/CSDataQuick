import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.2

Dialog {
    id: dialog
    title: 'Input Dialog'

    property string hint
    property string input

    standardButtons: StandardButton.Ok | StandardButton.Cancel

    Column {
        Text {
            text: hint
        }
        TextField {
            id: textField
            text: input
        }
    }

    onAccepted: {
        input = textField.text
    }
}
