import QtQuick.Controls 2.2

Menu {
    id: root

    function addItemByTitle(title) {
        var cmd = 'import QtQuick.Controls 2.0; MenuItem {text: "' + title + '"}'
        var item = Qt.createQmlObject(cmd, root, 'menuItem')
        root.addItem(item)
        return item
    }
}
