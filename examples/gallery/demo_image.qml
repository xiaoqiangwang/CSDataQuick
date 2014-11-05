import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaImage {
            id: oval
            width: 200
            height: 200
            source: 'http://www.animatedimages.org/data/media/75/animated-train-image-0017.gif'
        }
    }
}
