import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

Rectangle {
    Column {
        anchors.fill: parent
        CSCartesianPlot {
            id: plot
            width: 200
            height: 200
            title: titleInput.text
            xLabel: xLabelInput.text
            yLabel: yLabelInput.text
            model: ListModel {
                ListElement {
                    xchannel: 'x'
                    ychannel: 'y'
                }
                ListElement {
                    xchannel: ''
                    ychannel: 'sin'
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
                role: 'xchannel'
                title: 'X'
                width: 80
            }
            TableViewColumn {
                role: 'ychannel'
                title: 'Y'
                width: 80
            }
            TableViewColumn {
                role: 'foreground'
                title: 'color'
                width: 80
            }
            model: plot.model
            itemDelegate: styleData.column == 2 ? colorRect : textInput
        }
    }

    Component {
        id: colorRect
        Rectangle {
            color: plot.model.get(styleData.row).foreground
        }
    }

    Component {
        id: textInput
        TextInput {
            text: styleData.value 
            onAccepted: {
                switch (styleData.column) {
                    case 0:
                        plot.model.setProperty(styleData.row, "xchannel", text)
                    break
                    case 1:
                        plot.model.setProperty(styleData.row, "ychannel", text)
                    break
                }
            }
        }
    }
}
