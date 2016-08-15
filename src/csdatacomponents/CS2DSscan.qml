import QtQuick 2.1
import QtQuick.Controls 1.0

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

/*!
    \qmltype CS2DSscan
    \inqmlmodule CSDataQuick.Components
    \brief Display 2D sscan

    \l {http://www.aps.anl.gov/bcda/synApps/sscan/sscan.html}{sscan} is a flexible way to 
    move positioners through a series of positions and record detector data at each of the positions.

    The following PVs are relavent:
    \table
    \header
        \li SSCAN PV
        \li Description
    \row
        \li $(X)CPT
        \li Current point of X dimension
    \row
        \li $(X)NPTS
        \li Number of points of X dimension
    \row
        \li $(X)PnSP
        \li Start point of positioner n of X dimension
    \row
        \li $(X)PnEP
        \li End point of positioner n of X dimension
    \row
        \li $(X)EXSC
        \li Execute scan flag of X dimension
    \row
        \li $(Y)CPT
        \li Current point of Y dimension
    \row
        \li $(Y)NPTS
        \li Number of points of Y dimension
    \row
        \li $(Y)PnSP
        \li Start point of positioner n of Y dimension
    \row
        \li $(Y)PnEP
        \li End point of positioner n of Y dimension
    \row
        \li $(Y)EXSC
        \li Execute scan flag of Y dimension
    \row
        \li $(X)DnnDA
        \li End of scan detector array of X dimension
    \row
        \li $(X)DnnCV
        \li Current detector value of X dimension
    \endtable

    \qml
        CS2DSscan {
            scanX: 'MTEST:scan1'
            scanY: 'MTEST:scan2'
        }
    \endqml

*/

Plot {
    id: plot
    width: 400
    height: 300
    /*!
        Set the sscan record prefix of X dimension
    */
    property string scanX
    /*!
        Set the sscan record prefix of Y dimension
    */
    property string scanY
    /*!
        Set the sscan record detector number
     */
    property int numD: 1
    /*!
        Set the sscan record positioner number of X dimension
     */
    property int numX: 1
    /*!
        Set the sscan record positioner number of Y dimension
     */
    property int numY: 1

    QtObject {
        id: d
        /* X dimension */
        property var cptX: CSData {
            source: scanX + '.CPT'
            onValueChanged: {
                cm.setCell(d.cptX.value, d.cptY.value, d.detX.value)
                plot.replot()
            }
        }
        property var nptsX: CSData {
            source: scanX + '.NPTS'
        }
        property var pvX: CSData {
            source: scanX + Utils.format('.P%.0fPV', numX)
        }
        property var spX: CSData {
            source: scanX + Utils.format('.P%.0fSP', numX)
        }
        property var epX: CSData {
            source: scanX + Utils.format('.P%.0fEP', numX)
        }
        property var runX: CSData {
            source: scanX + '.EXSC'
            onValueChanged: {
                if (value == 1) {
                    cm.setXRange(d.spX.value, d.epX.value, d.nptsX.value)
                }
            }
        }
        /* Y dimension */
        property var cptY: CSData {
            source: scanY + '.CPT'
        }
        property var nptsY: CSData {
            source: scanY + '.NPTS'
        }
        property var pvY: CSData {
            source: scanY + Utils.format('.P%.0fPV', numY)
        }
        property var spY: CSData {
            source: scanY + Utils.format('.P%.0fSP', numY)
        }
        property var epY: CSData {
            source: scanY + Utils.format('.P%.0fEP', numY)
        }
        property var runY: CSData {
            source: scanY + '.EXSC'
            onValueChanged: {
                if (value == 1) {
                    cm.setYRange(d.spY.value, d.epY.value, d.nptsY.value)
                }
            }
        }
        /* Detector of X dimension */
        property var detX: CSData {
            source: scanX + Utils.format('.D%02.0fCV', numD)
        }
    }
    property var xAxis: Axis {
        type: Axis.Bottom
        label: d.pvX.value
        autoScale: true
    }
    property var yAxis: Axis {
        type: Axis.Left
        label: d.pvY.value
        autoScale: true
    }

    ColorMapItem {
        id: cm
        x: xAxis
        y: yAxis
    }
}
