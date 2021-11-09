pragma Singleton

import QtQml 2.0
import QtQuick 2.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Compat 1.0 as Compat

Compat.Dialog {
    id: displayDialog

    Compat.ScrollView {
        anchors.fill: parent
        ListView {
            id: listView
            contentWidth: contentItem.childrenRect.width
            flickableDirection: Flickable.AutoFlickDirection
            model: WindowManager.entries
            delegate: Compat.Label {
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
            section.delegate: Compat.Label {
                text: section
                font.bold: true
            }

            onCountChanged: {
                positionViewAtIndex(count-1, ListView.Beginning)
            }
        }
    }
}
