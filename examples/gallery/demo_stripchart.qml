import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

Rectangle {
    Column {
        anchors.fill: parent
        CSStripChart {
            id: plot
            width: 200
            height: 200
            title: titleInput.text
            xLabel: xLabelInput.text
            yLabel: yLabelInput.text
            model: ListModel {
                ListElement {
                    channel: 'calc'
                }
            }
        }
        Grid {
            columns: 2
            Text { text: "title"}
            TextField {id: titleInput}
            Text { text: "x label"}
            TextField {id: xLabelInput}
            Text { text: "y label"}
            TextField {id: yLabelInput}
        }
        TableView {
            TableViewColumn {
                role: 'channel'
                title: 'channel'
                width: 80
            }
            TableViewColumn {
                role: 'foreground'
                title: 'color'
                width: 80
            }
            model: plot.model
            itemDelegate: Loader {
                property QtObject styleDataExported: styleData
                sourceComponent: styleData.column == 1 ? colorRect : textInput
            }
        }
    }

    Component {
        id: colorRect
        Rectangle {
            color: plot.model.get(styleDataExported.row).foreground
        }
    }

    Component {
        id: textInput
        TextInput {
            text: styleDataExported.value 
            onAccepted: {
                switch (styleDataExported.column) {
                    case 0:
                        plot.model.setProperty(styleDataExported.row, "channel", text)
                    break
                }
            }
        }
    }
}
