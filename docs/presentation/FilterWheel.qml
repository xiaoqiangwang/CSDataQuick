import QtQuick 2.4
import CSDataQuick.Data 1.0

Item {
    width: 300
    height: 300
    Rectangle {
        id: wheel
        width: Math.min(parent.width, parent.height)
        height: width
        radius: width / 2
        color: 'black'
        Repeater {
            anchors.fill: parent
            model: filter.stateStrings
            Rectangle {
                property real angle: index * 60
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: (wheel.radius - radius) * Math.cos(angle / 180 * Math.PI)
                anchors.verticalCenterOffset: (wheel.radius - radius) * Math.sin(angle / 180 * Math.PI)
                width:  2*radius
                height: 2*radius
                radius: wheel.radius / 4
                color: -(90 + wheel.rotation) % 360 / 60 == index ? 'white' : 'gray'
                Text {
                    anchors.centerIn: parent
                    text: modelData
                    rotation: -wheel.rotation
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: filter.setValue(index)
                }
            }
        }
    }
    RotationAnimation {
        id: anime
        target: wheel
        running: false
    }

    CSData {
        id: filter
        source: "filter"
        property int oldValue: -1

        onValueChanged: {
            if (oldValue == -1) {
                wheel.rotation = -90 - value * 60
            } else {
                var diff = value - oldValue
                if (diff > 3)
                    diff -= 6
                else if (diff < -3)
                    diff += 6

                anime.to = wheel.rotation  - diff * 60
                anime.duration = 500 * Math.abs(diff)
                anime.running = true
            }
            oldValue = value
       }
    }
}
