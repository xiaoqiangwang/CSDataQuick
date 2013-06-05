// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import QtQuick.Controls 1.0

import PvLibrary 1.0

Rectangle {
    width: 360
    height: 360
    PvObject {
        id: pv1
        channel: "calc.SCAN"
        asstring:true
    }

    Column {
        x: 48
        y: 43

        TextInput {
            id: pv1_edit
            width:100
            height:24
            text: pv1.value

            onAccepted: {
                pv1.value = text
            }
        }

        Text {
            width: 100
            height: 20
            id: pv1_display

            enabled: pv1.connected
            text: pv1.value
        }

        ComboBox {
            id: pv1_combo
            width: 100
            height: 20
            Connections {
                target: pv1
                onConnectionChanged: {
                    if (pv1.connected) {
                        pv1_combo.model = pv1.strs
                    }
                }
                onValueChanged: {
                    pv1_combo.currentText = pv1.value
                }
            }
            onCurrentIndexChanged: {
                pv1.value = pv1_combo.currentIndex
            }
        }
    }
}
