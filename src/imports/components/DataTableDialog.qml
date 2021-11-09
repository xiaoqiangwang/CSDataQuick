pragma Singleton

import QtQuick 2.0
import QtQuick.Layouts 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components.Compat 1.0 as Compat

Compat.Dialog {
    id: root
    modal: false

    height: 230

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        Row {
            spacing: 5
            Layout.fillWidth: true
            Compat.Label {
                text: 'Select Engine'
            }
            Compat.ComboBox {
                id: engineCombo
                implicitWidth: 200
                model: DataEngineManager.engines
                textRole: 'description'
                property var engine:  DataEngineManager.engines[currentIndex]
            }
        }
        Compat.DataTable {
            id: pvTable
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: proxyModel
        }
        Compat.TextField {
            id: searchBox
            Layout.fillWidth: true
            Compat.Label {
                x: searchBox.focus ? 20 : (parent.width - implicitWidth) / 2
                anchors.verticalCenter: parent.verticalCenter
                text: "Look for a data source?"
                color: "gray"
                visible: searchBox.text == ''
                Behavior on x {
                    NumberAnimation {duration:200}
                }
            }
        }
        SortFilterProxyModel {
            id: proxyModel
            source: (root.visible && engineCombo.engine)? engineCombo.engine.allData : null

            //sortOrder: pvTable.sortIndicatorOrder
            sortCaseSensitivity: Qt.CaseInsensitive
            //sortRole: pvTable.getColumn(pvTable.sortIndicatorColumn).role

            filterString: searchBox.text
            filterRole: 'source'
            filterSyntax: SortFilterProxyModel.RegExp
            filterCaseSensitivity: Qt.CaseInsensitive
        }
    }
}

