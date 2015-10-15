pragma Singleton
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQml.Models 2.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

import CSDataQuick.Data 1.0

Dialog {
    id: root
    standardButtons: StandardButton.Ok
    modality: Qt.NonModal

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        Row {
            spacing: 5
            Layout.fillWidth: true
            Text {
                text: 'Select Engine'
            }
            ComboBox {
                id: engineCombo
                implicitWidth: 200
                model: DataEngineManager.engines
                textRole: 'description'
                onCurrentIndexChanged: pvTable.model = DataEngineManager.engines[currentIndex].allData
            }
        }
        TableView {
            id: pvTable
            Layout.fillWidth: true
            Layout.fillHeight: true
            TableViewColumn {
                title: 'Source'
                role: 'source'
            }
            TableViewColumn {
                title: 'Connection'
                role: 'connected'
                width: 80
                delegate: Text {
                    text: styleData.value ? 'connected' : 'disconnected'
                    color: styleData.value ? 'green' : 'red'
                }
            }
        }
    }
}

