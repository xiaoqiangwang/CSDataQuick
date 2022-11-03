import QtQuick.Controls 1.0

Menu {
    id: root

    function addItemByTitle(title) {
        return root.addItem(title)
    }
}
