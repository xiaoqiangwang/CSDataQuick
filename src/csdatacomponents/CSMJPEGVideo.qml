import QtQuick 2.1
import QtQuick.Controls 1.0

import CSDataQuick.Components 1.0

/*!
    \qmltype CSMJPEGVideo
    \inqmlmodule CSDataQuick.Components
    \brief Display a video from MJPEG video stream

    This is not related to CSData, but provided for convenience.
    \e{Note: The MJPEG stream header must specify Content-Length}

    \qml
        CSMJPEGVideo {
            source: ''
        }
    \endqml
*/

BaseItem {
    id: root
    /*!
        \qmlproperty string source
        This property holds the url to the MJPEG stream.
    */
    property alias source: mjpeg.source

    width: 400
    height: 300

    ImageItem {
        id: img
        anchors.fill: parent
    }

    MJPEG {
        id: mjpeg
        onImageChanged: img.setImage(image)
    }
}
