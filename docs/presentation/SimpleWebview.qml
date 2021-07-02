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
                    placeholderText: 'Search or enter website name'
                    inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhPreferLowercase
                    text: webView.url
                    Layout.fillWidth: true

                    onAccepted: {
                        var url
                        if (text.search(/https?:\/\//) === 0)
                            url = text
                        else if (text.search(' ') !== -1 || text.search('.') === -1) {
                            url = 'https://duckduckgo.com/?q=' + text.replace(' ','+')
                        } else {
                            url = 'http://' + text
                        }
                        webView.url = url
                    }

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
