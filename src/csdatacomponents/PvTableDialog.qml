pragma Singleton
import QtQuick 2.0
import QtQuick.Controls 1.0
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
                property var engine:  DataEngineManager.engines[currentIndex]
            }
        }
        TableView {
            id: pvTable
            Layout.fillWidth: true
            Layout.fillHeight: true
            sortIndicatorVisible: true
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
            property var sourceModel: engineCombo.engine.allData
            model: sourceModel.filter(filterByName).sort(sortByObject)
        }
        TextField {
            id: searchBox
            Layout.fillWidth: true
            Text {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                text: "Look for a data source?"
                visible: searchBox.text == '' && searchBox.focus == false
            }
        }
    }
    function filterByName (value, index, array) {
        return (value.source.indexOf(searchBox.text) != -1)
    }
    function compare(first, second, inverse) {
        if (first < second)
            return inverse ? 1 : -1
        if (first > second)
            return inverse ? -1 : 1
        return 0
    }
    function sortByObject(first, second) {
        if (pvTable.sortIndicatorColumn == 0) {
            return compare(first.source, second.source, pvTable.sortIndicatorOrder == Qt.AscendingOrder)
        } else if (pvTable.sortIndicatorColumn == 1) {
            return compare(first.connected, second.connected, pvTable.sortIndicatorOrder == Qt.AscendingOrder)
        }
    }
}

