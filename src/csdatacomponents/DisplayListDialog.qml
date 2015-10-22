pragma Singleton

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
            width: displayDialog.width
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
                text: model.modelData.macro ? model.modelData.macro : '<<No Macro Substitute>>'
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var window = model.modelData.window
                        window.raise()
                        window.requestActivate()
                    }
                }
            }
            section.property: "modelData.filePath"
            section.criteria: ViewSection.FullString
            section.delegate: sectionHeading

            onCountChanged: {
                var newIndex = count - 1 // last index
                positionViewAtEnd()
                currentIndex = newIndex
            }
        }
    }
    standardButtons: StandardButton.Ok
}
