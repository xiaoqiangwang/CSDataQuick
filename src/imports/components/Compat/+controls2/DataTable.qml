import QtQuick 2.12
import QtQuick.Controls 2.2
import Qt.labs.qmlmodels 1.0

TableView {
    id: root

    ScrollBar.vertical: ScrollBar {
    }

    columnWidthProvider: function (column) {
        return d.columnWidths[column]
    }

    topMargin: horizontalHeader.implicitHeight

    delegate: DelegateChooser {
        DelegateChoice {
            column: 1
            delegate: Label {
                text: model.source
            }
        }

        DelegateChoice {
            column: 2
            delegate: Label {
                text: model.connected ? 'connected' : 'disconnected'
                color: model.connected ? 'green' : 'red'
            }
        }

    }

    Row {
        id: horizontalHeader
        y: root.contentY
        z: 2

        Label {
            text: 'Source'
            width: d.columnWidths[1]
            opacity: 1
            background: Rectangle {color: palette.base}
        }
        Label {
            text: 'Connection'
            opacity: 1
            background: Rectangle {color: palette.base}
        }
    }

    QtObject {
        id: d
        property var columnWidths: [0, 200, 50]
    }
}
