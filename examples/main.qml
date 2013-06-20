// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.1
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    width: 360
    height: 360


    Column {
        x: 48
        y: 100


       
        CaMessageButton {
            width: 100
            height: 24
            text: 'Click Me!'
            channel: 'catest'
            onMessage: 45
            offMessage: 34
        }

        CaSlider {
            id: pv2_slider
            width: 100
            height: 24
            channel: 'catest'
        }

        CaBar {
            id: pv2_bar
            width: 100
            height: 24
            channel: 'catest'
        }

        CaTextEntry {
            id: pv1_edit
            width:100
            height:24
            channel: 'catest'
        }

        CaTextLabel {
            id: pv1_display
            width: 100
            height: 20
            channel: 'catest'
            alarmColor: true
        }

        CaMenu {
            id: pv1_combo
            width: 100
            height: 20
            channel: 'calc.SCAN'
        }
        CaChoiceButton {
            orientation: 0
            channel: 'calc.SCAN'
        }
 
        CaOval {
            id: pv1_oval
            width: 100
            height: 70
        }
    }
}
