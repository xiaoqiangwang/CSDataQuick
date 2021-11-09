import QtQuick 2.0
import QtQuick.Layouts 1.0

import CSDataQuick.Components 1.0
import CSDataQuick.Components.Compat 1.0 as Compat

Compat.Dialog {
    id: root

    property string channel
    property Limits limits: Limits {}

    withCancel: true

    title: 'Data Limits'

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

    GridLayout {
        id: mainLayout
        anchors.fill: parent
        columns: 3
        rowSpacing: 2
        anchors.margins: 5

        Compat.Label {
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
                border.color: parent.color
                border.width: 1
                color: 'transparent'
            }
            font.pixelSize: 15
        }

        Compat.Label {
            text: 'Source'
            horizontalAlignment: Text.AlignHCenter
            Layout.row: 1
            Layout.column: 1
        }

        Compat.Label {
            text: 'Value'
            horizontalAlignment: Text.AlignHCenter
            Layout.row: 1
            Layout.column: 2
        }

        /* Low Limit */
        Compat.Label {
            text: 'Low Limit'
            horizontalAlignment: Text.AlignRight
            width: 100
        }
        Compat.ComboBox {
            model: sourceModel
            textRole: "text"
            currentIndex: root.limits.loprSrc
            onCurrentIndexChanged: {
                root.limits.loprSrc = sourceModel.get(currentIndex).value
            }
        }
        Compat.TextField {
            id: lopr
            implicitWidth: 80
            text: root.limits.lopr.toFixed(root.limits.prec)
            validator: DoubleValidator {}
            enabled: root.limits.loprSrc == LimitsSource.User
            onAccepted: root.limits.loprUser = parseFloat(text)
        }

        /* High Limit */
        Compat.Label {
            text: 'High Limit'
            horizontalAlignment: Text.AlignRight
            width: 100
        }
        Compat.ComboBox {
            model: sourceModel
            textRole: "text"
            currentIndex: root.limits.hoprSrc
            onCurrentIndexChanged: {
                root.limits.hoprSrc = sourceModel.get(currentIndex).value
            }
        }
        Compat.TextField {
            id: hopr
            implicitWidth: 80
            text: root.limits.hopr.toFixed(root.limits.prec)
            validator: DoubleValidator {}
            enabled: root.limits.hoprSrc == LimitsSource.User
            onAccepted: root.limits.hoprUser = parseFloat(text)
        }

        /* Precision */
        Compat.Label {
            text: 'Precision'
            horizontalAlignment: Text.AlignRight
            width: 200
        }
        Compat.ComboBox {
            model: sourceModel
            textRole: "text"
            currentIndex: root.limits.precSrc
            onCurrentIndexChanged: {
                root.limits.precSrc = sourceModel.get(currentIndex).value
            }
        }
        Compat.TextField {
            id: prec
            implicitWidth: 80
            text: root.limits.prec.toString()
            validator: IntValidator {top: 17; bottom: 0}
            enabled: root.limits.precSrc == LimitsSource.User
            onAccepted: root.limits.precUser = parseInt(text)
        }
    }
}
