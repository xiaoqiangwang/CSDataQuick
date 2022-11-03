import QtQuick 2.0
import QtQuick.Controls 1.0

import CSDataQuick.Data 1.0

TableView {
    id: pvTable
    sortIndicatorVisible: true

    TableViewColumn {
        title: 'Source'
        role: 'source'
    }

    TableViewColumn {
        title: 'Connection'
        role: 'connected'
        width: 80
        delegate: Label {
            text: styleData.value ? 'connected' : 'disconnected'
            color: styleData.value ? 'green' : 'red'
        }
    }
}
