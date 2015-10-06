import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import Qt.labs.folderlistmodel 2.0

import CSDataQuick.Components 1.0

Rectangle {
    id: root
    width: 800
    height: 600

    FolderListModel {
        id: qmlFiles
        nameFilters: ["demo_*.qml"]
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 3

        TableView {
            id: listView
            Layout.fillHeight: true
            width: 100

            model: qmlFiles

            TableViewColumn {
                delegate: Text {
                    text: model.fileBaseName.substr(5)
                    color: styleData.textColor
                }
            }
        }

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: listView.currentRow == -1 ? '' : qmlFiles.get(listView.currentRow, 'fileURL')
        }
    }
}
