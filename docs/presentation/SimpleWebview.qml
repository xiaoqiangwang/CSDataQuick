import QtQuick 2.0
import QtWebEngine 1.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {

    ColumnLayout{
        anchors.fill: parent
        ToolBar {
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    text: '<'
                    onClicked: webView.goBack()
                    enabled: webView.canGoBack
                }
                ToolButton {
                    text: '>'
                    onClicked: webView.goForward()
                    enabled: webView.canGoForward
                }
                TextField {
                    id: urlField
                    inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhPreferLowercase
                    text: webView.url
                    Layout.fillWidth: true

                    onAccepted: webView.url = text

                    ProgressBar {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: parent.font.pixelSize / 5
                        visible: webView.loading
                        minimumValue: 0
                        maximumValue: 100
                        value: webView.loadProgress == 100 ? 0 : webView.loadProgress
                    }
                }
                ToolButton {
                    text: 'Go'
                    onClicked: webView.url = urlField.text
                }
            }
        }

        WebEngineView {
            id: webView
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
