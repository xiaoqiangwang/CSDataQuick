pragma Singleton

import QtQml 2.0
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.2

import CSDataQuick.Components 1.0

Dialog {
    id: displayDialog
    modality: Qt.NonModal
    // content item is a window list viewer
    Component {
        id: sectionHeading
        Rectangle {
            width: parent.width
            height: childrenRect.height
            color: "lightsteelblue"
            Text {
                text: section
                font.bold: true
            }
        }
    }

    contentItem: ScrollView {
        ListView {
            model: WindowManager.entries
            delegate: Text {
                width: parent.width
                wrapMode: Text.WordWrap
                text: macro ? macro : '<<No Macro Substitute>>'
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        window.raise()
                        window.requestActivate()
                    }
                }
            }
            section.property: "filePath"
            section.criteria: ViewSection.FullString
            section.delegate: sectionHeading

            onCountChanged: {
                positionViewAtIndex(count-1, ListView.Beginning)
            }
        }
    }
    standardButtons: StandardButton.Ok
}
