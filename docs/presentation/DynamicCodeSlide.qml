import QtQuick 2.5

Slide {
    id: root

    property alias code: code_editor.text
    property alias readOnly: code_editor.readOnly
    property string codeFontFamily: parent.codeFontFamily;
    property real codeFontSize: baseFontSize * 0.6;
    property bool isDynamicCodeSlide: true
    property bool segment: false

    QtObject {
        id: d
        property var rootItem: null
    }
    
    Item {
        id: item_view
        width: parent.width / 2
        height: parent.height
    }

    CodeSection {
        id: code_editor
        readOnly: false
        text: ""
    }

    function executeCode() {
        if (d.rootItem !== null) {
            d.rootItem.destroy()
            d.rootItem = null
        }
        var source = code_editor.text
        if (segment)
            source = 'import QtQuick 2.5\nItem {\nanchors.fill: parent\n' + source + '\n}'
        d.rootItem = Qt.createQmlObject(source, item_view, '')
        d.rootItem.anchors.centerIn = item_view
    }

    Component.onCompleted: {
        if (code_editor.text)
            executeCode()
    }
}


