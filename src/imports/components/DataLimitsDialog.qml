import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

import CSDataQuick.Components 1.0

Dialog {
    id: root

    property string channel
    property Limits limits: Limits {}

    width: 300
    height: 180

    title: 'Data Limits'

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
                text: root.channel
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
                currentIndex: root.limits.loprSrc
                onCurrentIndexChanged: {
                    root.limits.loprSrc = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                id: lopr
                implicitWidth: 80
                text: root.limits.lopr.toFixed(root.limits.prec)
                validator: DoubleValidator {}
                enabled: root.limits.loprSrc == LimitsSource.User
                onAccepted: root.limits.loprUser = parseFloat(text)
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
                currentIndex: root.limits.hoprSrc
                onCurrentIndexChanged: {
                    root.limits.hoprSrc = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                id: hopr
                implicitWidth: 80
                text: root.limits.hopr.toFixed(root.limits.prec)
                validator: DoubleValidator {}
                enabled: root.limits.hoprSrc == LimitsSource.User
                onAccepted: root.limits.hoprUser = parseFloat(text)
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
                currentIndex: root.limits.precSrc
                onCurrentIndexChanged: {
                    root.limits.precSrc = sourceModel.get(currentIndex).value
                }
            }
            TextField {
                id: prec
                implicitWidth: 80
                text: root.limits.prec.toString()
                validator: IntValidator {top: 17; bottom: 0}
                enabled: root.limits.precSrc == LimitsSource.User
                onAccepted: root.limits.precUser = parseInt(text)
            }

            /* Button */
            Button {
                id: ok
                text: 'OK'
                onClicked: {
                    root.accepted()
                    root.close()
                }
            }
            Button {
                id: cancel
                text: 'Cancel'
                onClicked: {
                    root.rejected()
                    root.close()
                }
            }
        }
    }
}
