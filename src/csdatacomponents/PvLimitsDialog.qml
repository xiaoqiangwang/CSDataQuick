import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

import CSDataQuick.Components 1.0

Dialog {
    id: dialog

    property string channel
    property Limits limits: Limits {}

    width: 300
    height: 180

    title: 'PV Limits'

    SystemPalette { id: palette; colorGroup: SystemPalette.Active }

    ListModel {
        id: sourceModel
        ListElement {
            text: 'Channel';
            value: LimitsSource.Channel;
        }
        ListElement {
            text: 'Default';
            value: LimitsSource.Default;
        }
        ListElement {
            text: 'User Specified';
            value: LimitsSource.User;
        }
    }

    contentItem: Rectangle{
        color: palette.window
        GridLayout {
            anchors.fill: parent
            columns: 3
            rowSpacing: 2
            anchors.margins: 5

            Text {
                text: channel
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                height: 25
                Layout.columnSpan: 3
                Layout.fillHeight: false
                Layout.fillWidth: true
                Rectangle {
                    z: -1
                    anchors.fill: parent
                    border.width: 1
                }
                font.pixelSize: 15
            }

            Text {
                text: 'Source'
                horizontalAlignment: Text.AlignHCenter
                Layout.row: 1
                Layout.column: 1
            }

            Text {
                text: 'Value'
                horizontalAlignment: Text.AlignHCenter
                Layout.row: 1
                Layout.column: 2
            }

            /* Low Limit */
            Text {
                text: 'Low Limit'
                horizontalAlignment: Text.AlignRight
                width: 100
            }
            ComboBox {
                model: sourceModel
                textRole: "text"
                currentIndex: limits.loprSrc
                onCurrentIndexChanged: {
                    limits.loprSrc = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                id: lopr
                implicitWidth: 80
                text: limits.lopr.toFixed(limits.prec)
                validator: DoubleValidator {}
                enabled: limits.loprSrc == LimitsSource.User
                onAccepted: limits.loprUser = parseFloat(text)
            }

            /* High Limit */
            Text {
                text: 'High Limit'
                horizontalAlignment: Text.AlignRight
                width: 100
            }
            ComboBox {
                model: sourceModel
                textRole: "text"
                currentIndex: limits.hoprSrc
                onCurrentIndexChanged: {
                    limits.hoprSrc = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                id: hopr
                implicitWidth: 80
                text: limits.hopr.toFixed(limits.prec)
                validator: DoubleValidator {}
                enabled: limits.hoprSrc == LimitsSource.User
                onAccepted: limits.hoprUser = parseFloat(text)
            }

            /* Precision */
            Text {
                text: 'Precision'
                horizontalAlignment: Text.AlignRight
                width: 200
            }
            ComboBox {
                model: sourceModel
                textRole: "text"
                currentIndex: limits.precSrc
                onCurrentIndexChanged: {
                    limits.precSrc = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                id: prec
                implicitWidth: 80
                text: limits.prec.toString()
                validator: IntValidator {top: 17; bottom: 0}
                enabled: limits.precSrc == LimitsSource.User
                onAccepted: limits.precUser = parseInt(text)
            }

            /* Button */
            Button {
                id: ok
                text: 'OK'
                onClicked: {
                    dialog.accepted()
                    close()
                }
            }
            Button {
                id: cancel
                text: 'Cancel'
                onClicked: {
                    dialog.rejected()
                    close()
                }
            }
        }
    }
}
