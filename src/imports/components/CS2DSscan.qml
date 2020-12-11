import QtQuick 2.1

import CSDataQuick.Data 1.0
import CSDataQuick.Components 1.0

/*!
    \qmltype CS2DSscan
    \inqmlmodule CSDataQuick.Components
    \ingroup csdataquick.components.monitors
    \brief Display 2D sscan.

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

    \image sscan.png

*/

BaseItem{
    id: root
    /*! This property holds the graph title */
    property string title
    /*! This property holds the sscan record prefix of X dimension */
    property string scanX
    /*! This property holds the sscan record prefix of X dimension */
    property string scanY
    /*! This property holds the sscan record detector number of X dimension */
    property int detX: 1
    /*! This property holds the sscan record positioner number of X dimension */
    property int posX: 1
    /*! This property holds the sscan record positioner number of Y dimension */
    property int posY: 1

    implicitWidth: 300
    implicitHeight: 200
    background: ColorMap.monitors_background
    foreground: ColorMap.foreground

    QtObject {
        id: d
        /* Detector of X dimension */
        property var detX: CSData {
            source: root.scanX + Utils.format('.D%02.0fCV', root.detX)
        }
        /* X dimension */
        property var valX: CSData {
            source: root.scanX + '.VAL'
            onValueChanged: {
                if (d.cptX.value === 0)
                    return
                cm.setCell(d.cptX.value - 1, d.cptY.value, d.detX.value)
                plot.replot()
            }
        }
        property var cptX: CSData {
            source: root.scanX + '.CPT'
        }
        property var nptsX: CSData {
            source: root.scanX + '.NPTS'
        }
        property var pvX: CSData {
            source: root.scanX + Utils.format('.P%.0fPV', root.posX)
        }
        property var spX: CSData {
            source: root.scanX + Utils.format('.P%.0fSP', root.posX)
        }
        property var epX: CSData {
            source: root.scanX + Utils.format('.P%.0fEP', root.posX)
        }
        property var runX: CSData {
            source: root.scanX + '.EXSC'
            onValueChanged: {
                if (value === 1) {
                    cm.setXRange(d.spX.value, d.epX.value, d.nptsX.value)
                }
            }
        }
        /* Y dimension */
        property var cptY: CSData {
            source: root.scanY + '.CPT'
        }
        property var nptsY: CSData {
            source: root.scanY + '.NPTS'
        }
        property var pvY: CSData {
            source: root.scanY + Utils.format('.P%.0fPV', root.posY)
        }
        property var spY: CSData {
            source: root.scanY + Utils.format('.P%.0fSP', root.posY)
        }
        property var epY: CSData {
            source: root.scanY + Utils.format('.P%.0fEP', root.posY)
        }
        property var runY: CSData {
            source: root.scanY + '.EXSC'
            onValueChanged: {
                if (value === 1) {
                    cm.clearData()
                    cm.setYRange(d.spY.value, d.epY.value, d.nptsY.value)
                }
            }
        }
    }

    Plot {
        id: plot
        anchors.fill: parent
        foreground: root.foreground
        background: root.background
        title: root.title

        property var xAxis: Axis {
            type: Axis.Bottom
            label: d.pvX.value || ''
            autoScale: true
        }
        property var yAxis: Axis {
            type: Axis.Left
            label: d.pvY.value || ''
            autoScale: true
        }

        ColorMapItem {
            id: cm
            x: plot.xAxis
            y: plot.yAxis
        }
    }
}
