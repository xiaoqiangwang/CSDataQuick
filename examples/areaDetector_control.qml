import QtQuick 2.5
import CSDataQuick.Components 1.0

BaseWindow {
    width: 800

    CSADImage {
        x: 439
        y: 46
        width: 330
        height: 330
        source: "$(P)$(I)"
    }

    CSText {
        id: text1
        x: 30
        y: 8
        width: 733
        height: 32
        text: "Area Detector Control $(P)$(R)"
        align: Text.AlignHCenter
    }

    CSText {
        id: text2
        x: 69
        y: 268
        width: 86
        height: 20
        text: "Acquire"
        align: Text.AlignRight
    }

    CSMessageButton {
        id: messageButton
        x: 173
        y: 268
        width: 67
        height: 20
        text: "Start"
        onMessage: "1"
        source: "$(P)$(R)Acquire"
    }

    CSMessageButton {
        id: messageButton1
        x: 246
        y: 268
        width: 67
        height: 20
        text: "Stop"
        onMessage: "0"
        source: "$(P)$(R)Acquire"
    }

    CSText {
        id: text3
        x: 197
        y: 242
        width: 106
        height: 20
        text: "Collecting"
        foreground: "#fbf34a"
        dynamicAttribute.channel: "$(P)$(R)Acquire"
        dynamicAttribute.visibilityMode: VisibilityMode.IfNotZero
    }

    CSText {
        id: text4
        x: 197
        y: 242
        width: 106
        height: 20
        text: "Done"
        foreground: "#1ebb00"
        align: Text.AlignHCenter
        dynamicAttribute.channel: "$(P)$(R)Acquire"
        dynamicAttribute.visibilityMode: VisibilityMode.IfZero
    }

    CSText {
        id: text5
        x: 44
        y: 106
        width: 130
        height: 20
        text: "Exposure time"
        align: Text.AlignRight
    }

    CSTextEntry {
        id: textEntry
        x: 180
        y: 106
        width: 60
        source: "$(P)$(R)AcquireTime"
    }

    CSText {
        id: text6
        x: 44
        y: 126
        width: 130
        text: "Acquire period"
        align: Text.AlignRight
    }

    CSTextEntry {
        id: textEntry1
        x: 180
        y: 126
        width: 60
        source: "$(P)$(R)AcquirePeriod"
    }

    CSTextUpdate {
        id: textUpdate
        x: 246
        y: 106
        width: 81
        height: 20
        source: "$(P)$(R)AcquireTime_RBV"
    }

    CSTextUpdate {
        id: textUpdate1
        x: 246
        y: 126
        width: 80
        height: 20
        source: "$(P)$(R)AcquirePeriod_RBV"
    }

    CSText {
        id: text7
        x: 44
        y: 152
        width: 130
        text: "# Images"
        align: Text.AlignRight
    }

    CSTextEntry {
        id: textEntry2
        x: 180
        y: 152
        width: 60
        source: "$(P)$(R)NumImages"
    }

    CSTextUpdate {
        id: textUpdate2
        x: 246
        y: 152
        width: 80
        height: 20
        source: "$(P)$(R)NumImages_RBV"
    }

    CSText {
        id: text8
        x: 7
        y: 178
        width: 170
        height: 20
        text: "# Images complete"
        align: Text.AlignRight
    }

    CSTextUpdate {
        id: textUpdate3
        x: 246
        y: 178
        width: 80
        height: 20
        source: "$(P)$(R)NumImagesCounter_RBV"
    }

    CSText {
        id: text9
        x: 7
        y: 204
        width: 100
        height: 20
        text: "Image mode"
        align: Text.AlignRight
    }

    CSMenu {
        id: menu
        x: 120
        y: 204
        width: 100
        height: 20
        source: "$(P)$(R)ImageMode"
    }

    CSTextUpdate {
        id: textUpdate4
        x: 227
        y: 204
        width: 100
        height: 20
        source: "$(P)$(R)ImageMode_RBV"
    }

    CSCartesianPlot {
        id: cartesianPlot
        x: 417
        y: 375
        width: 368
        height: 100
        model: ListModel {
            ListElement {
                ychannel: '$(P)$(S)ProfileAverageX_RBV'
            }
        }
    }

    CSCartesianPlot {
        id: cartesianPlot1
        x: 199
        y: 164
        width: 368
        height: 100
        rotation: -90
        model: ListModel {
            ListElement {
                ychannel: "$(P)$(S)ProfileAverageY_RBV"
            }
        }
    }

    CSText {
        id: text10
        x: 8
        y: 294
        width: 147
        height: 20
        text: "Detector state"
        align: Text.AlignRight
    }

    CSTextUpdate {
        id: textUpdate5
        x: 173
        y: 294
        width: 133
        height: 20
        colorMode: ColorMode.Alarm
        source: "$(P)$(R)DetectorState_RBV"
    }

    CSText {
        id: text11
        x: 8
        y: 320
        width: 70
        height: 20
        text: "Status"
    }

    CSTextUpdate {
        id: textUpdate6
        x: 89
        y: 320
        width: 224
        height: 20
        format: TextFormat.String
        colorMode: ColorMode.Alarm
        source: "$(P)$(R)StatusMessage_RBV"
    }

    CSText {
        id: text12
        x: 8
        y: 346
        width: 147
        height: 20
        text: "Time remaining"
        align: Text.AlignRight
    }

    CSTextUpdate {
        id: textUpdate7
        x: 173
        y: 346
        width: 133
        height: 20
        colorMode: ColorMode.Alarm
        source: "$(P)$(R)TimeRemaining_RBV"
    }

    CSText {
        id: text13
        x: 25
        y: 378
        width: 130
        text: "Image counter"
        align: Text.AlignRight
    }

    CSTextEntry {
        id: textEntry3
        x: 173
        y: 378
        width: 60
        source: "$(P)$(R)ArrayCounter"
    }

    CSTextUpdate {
        id: textUpdate8
        x: 239
        y: 378
        width: 80
        height: 20
        source: "$(P)$(R)ArrayCounter_RBV"
    }

    CSText {
        id: text14
        x: 8
        y: 404
        width: 147
        height: 20
        text: "Image rate"
        align: Text.AlignRight
    }

    CSTextUpdate {
        id: textUpdate9
        x: 173
        y: 404
        width: 133
        height: 20
        colorMode: ColorMode.Alarm
        source: "$(P)$(R)ArrayRate_RBV"
    }

    CSRelatedDisplay {
        id: relatedDisplay
        x: 69
        y: 435
        model: [{"file":"commonPlugins.adl","label":"-All","macro":"P=$(P)","replace":false}]
    }

    CSText {
        id: text15
        x: 11
        y: 435
        width: 52
        height: 20
        text: "All"
    }

}
