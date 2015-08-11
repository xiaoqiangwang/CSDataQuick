import QtQuick 2.1
import QtQuick.Controls 1.0
import PvComponents 1.0

/*!
    \qmltype CaADImage
    \inqmlmodule PvComponents
    \brief Display an image from areaDetector

    \l{http://cars9.uchicago.edu/software/epics/areaDetector.html}
    {areaDetector} is a generic EPICS framework to integrate detectors.
    The detector data is internally represented as a \l
    {http://cars9.uchicago.edu/software/epics/areaDetectorDoc.html#NDArray}
    {NDArray} structure. The plugin \l
    {http://cars9.uchicago.edu/software/epics/NDPluginStdArrays.html}
    {NDPluginStdArrays} makes it available as EPICS PVs.

    Because EPICS PV only supports 1-D waveform. To reconstruct the image.
    The following PVs are relavent:
    \table
    \header
        \li areaDetector PV
        \li Description
    \row
        \li $(P)$(R)ArraySize0_RBV
        \li First dimension of NDArray data
    \row
        \li $(P)$(R)ArraySize1_RBV
        \li Second dimension of NDArray data
    \row
        \li $(P)$(R)ArraySize2_RBV
        \li Third dimension of NDArray data
    \row
        \li $(P)$(R)DataType_RBV
        \li Data type of NDArray data
    \endtable

    \qml
        CaADImage {
            interval: 200
            channel: '13SIM1:image1:'
        }
    \endqml

    \image ADImage.gif
*/

Image {
    /*!
        \qmlproperty real interval
        Set the interval between image refresh, in milliseconds.
    */
    property alias interval: timer.interval
    /*! Set the NDPluginStdArrays prefix. */
    property string channel

    cache: false
    fillMode: Image.PreserveAspectFit

    Timer {
        id: timer
        running: true
        repeat: true
        onTriggered: {
            source = ''
            source = 'image://areaDetector/'+ channel
        }
    }
}
