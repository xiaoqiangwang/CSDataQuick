import QtQuick 2.0
import QtQuick.Controls 1.0 as Controls
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.0

import HelperWidgets 2.0

Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("Colors")

    SectionLayout {
        columns: 2
        Label {
            text: 'foreground'
        }
        ColorPalette {
            backendValue: backendValues.foreground
        }
        Label {
            text: 'background'
        }
        ColorPalette {
            backendValue: backendValues.background
        }
    }
}

