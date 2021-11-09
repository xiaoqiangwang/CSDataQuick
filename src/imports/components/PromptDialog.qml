import QtQuick 2.0

import CSDataQuick.Components.Compat 1.0 as Compat

Compat.Dialog {
    id: root
    title: 'Input Dialog'

    property string hint
    property string input

    withCancel: true

    Column {
        Compat.Label {
            text: root.hint
        }
        Compat.TextField {
            id: textField
            text: root.input
        }
    }

    onAccepted: {
        root.input = textField.text
    }
}
