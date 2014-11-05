import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import PvComponents 1.0

Rectangle {
    ColumnLayout {
        anchors.fill: parent
        CaPolygon {
            id: polygon
            width: 201
            height: 201
            points: [Qt.point(50,0), Qt.point(0, 100), Qt.point(50, 200), Qt.point(150, 200), Qt.point(200,100)]
            edge: edgeStyleGroup.edgeStyle
            fill: fillStyleGroup.fillStyle

        }
        EdgeStyleGroup {
            id: edgeStyleGroup
        }
        FillStyleGroup {
            id: fillStyleGroup
        }
    }
}
