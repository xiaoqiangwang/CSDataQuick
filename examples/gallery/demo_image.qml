import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CSImage {
            id: oval
            width: 200
            height: 200
            source: 'http://www.animatedimages.org/data/media/75/animated-train-image-0017.gif'
        }
    }
}
