import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import CSDataQuick.Components 1.0

Presentation
{
    id: presentation

    width: 1200
    height: 800

    showNotes: true
    mouseNavigation: false
    arrowNavigation: false
    numberNavigation: false
    buttonNavigation: true

    SlideCounter {}
    Clock {}

    Slide {
        id: master
        anchors.fill: parent
        anchors.bottomMargin: parent.height * 0.1
        Image {
            id: banner
            anchors.top: parent.top
            source: "PSI Aerial.png"
            width: parent.width
            height: parent.width / 3
        }
        Text {
            width: parent.width
            anchors.bottom: parent.bottom
            anchors.top: banner.bottom
            anchors.topMargin: master.fontSize * 1.5
            font.pixelSize: master.baseFontSize
            font.family: fontFamily
            color: textColor
            wrapMode: Text.Wrap
            textFormat: Text.RichText

            text: '
                <p style="text-align:center;line-height:150%;">EPICS integration in QML/QtQuick<br/>
                Dynamic GUI in the <i><b>Quick</b></i> Way<br/></p>
                <p style="text-align:right;font-size:small;">Xiaoqiang Wang<br>EPICS Collabration Meeting July 2021</p>
            '
        }
        notes: '
        You are all familiar with Qt since the universal introduction of caqtdm.

        Today I am going to show another part of Qt, which is quite different from the classic Qt.
        '
    }

    Slide {
        title: 'History and Now'

        notes: '
        Nokia failed but QML/QtQuick survived.
        
        Applications range from big to small screens.

        Web is a new battle field for Qt. Come back to that later.
        '
        content: [
        "aka Qt Declarative as in qt repository and packages",
        "Started in the era of Nokia", 
        " QtQuick 1 in Qt 4.7",
        " QtQuick 2.x in Qt 5.x",
        " Version-less since Qt 6",
        "Platform coverage",
        " Mobile apps",
        " Embedded system, e.g. HMI, IVI", 
        " Desktop applications, e.g. KDE Plasma 5",
        " Web applications"
        ]
    }

    DynamicCodeSlide {
        title: "Hello QtQuick"
        notes: '
        import statement

        Structural text, literal object in JavaScript.
        Named type, properties

        Properties: color, font, rich text
        '
        code: '
import QtQuick 2.5

Text {
    text: "Hello QtQuick!"
}
'
    }

    Slide {
        title: "QtQuick/QML"
        content: [
            "QML",
            " The language and the infrastructure engine",
            " Declarative, JSON-like",
            " Javascript expression for dynamic property binding",
            " Extensible with C++",
            "QtQuick",
            " QQuickItem derived visual items",
            " QQuickPaintedItem with QPainter API provided for convenience",
            " Scene Graph with multiple render backends, software, OpenGL, vulkan, metal, d3d",
        ]

        notes:'
        Qt Model Language, Qt Markup Language.
        
        Item described by its geometry/material.

        Now I will intruduce some key aspect through examples.
        '
    }

    DynamicCodeSlide {
        title: "Property Binding"
        notes: '
        Relationship between different properties from different objects.
        '
        readOnly: false
        code: '
import QtQuick 2.5

Item {
  Rectangle {
    id: box_red
    width: 80
    height: 80
    color: "red"

    MouseArea {
      anchors.fill: parent
      drag.target: parent
    }
  }

  Rectangle {
    id: box_blue
    x: 80
    width: 80
    height: 80
    color: "blue"

    MouseArea {
      anchors.fill: parent
      drag.target: parent
    }
  }
}
    '
    }

    DynamicCodeSlide {
        title: "States/Transition/Animation"
        notes: '
        A state is set of property settings, e.g. color and font size.

        Animation can smooth the transition from/to state.
 
        Together this creates a fluid user experience.
        '
        code: '
import QtQuick 2.5

TextInput {
    id: helloText
    text: "Hello QtQuick!"

    states: State {
        when: helloText.focus
        PropertyChanges { target: helloText; font.pixelSize: 30 }
        PropertyChanges { target: helloText; color: "red"}
    }
    transitions: Transition {
        NumberAnimation {property: "font.pixelSize"; duration: 500}
        ColorAnimation {duration: 1000}
    }
}
        '
    }

    DynamicCodeSlide {
        title: "Model/View/Delegate"
        notes: '
        Model/View/Delegate.

        Model contains the data, identified by role names.
        View organises the data display.
        Delegate decides how the data appears in the view.

        Json object as model.
        '
        segment: true
        code: '
ListModel {
    id: fruitModel
    ListElement {name: "Apple"; color: "red"}
    ListElement {name: "Banana"; color: "yellow"}
    ListElement {name: "Lime"; color: "green"}
}

ListView {
    height: 100
    model: fruitModel
    anchors.centerIn: parent
    delegate: Text {
        text: name
    }
}
'
    }

    Slide {
        centeredText: "E P I C S ?"
        notes: '
        or control system data in general
        '
    }

    Slide {
        title: "Introduce CSDataQuick"
        notes: '
        name inspired by Control System Studio.
        Although it does not (yet) support any control systems other than EPICS CA,
        but there is nothing wrong in dreaming big. At least it will not be limited by the name.

        QCSData modeled after EPICS PV.
        QCSDataEngine is an interface to creates/updates/deletes QCSData.
        Specific data engines are implmented as plugins. This concept has the root in EDM designs.

        ADL is best supported. Common features in EDL and UI.
        '
        content: [
            "C++ libararies",
            " csdata",
            "  QCSData object: source, value, fieldType, alarm, units, timeStamp, precision, range, accessRight",
            "  QCSDataEngine plugins: ca://, sim:// and loc://",
            " csparser: Input parser for ADL, EDL and UI(caqtdm) files",
            " csutils: Helper functions",
            "QML modules",
            " CSDataQuick.Data: QCSData registered to QML engine",
            " CSDataQuick.Components: visual items",
            "Applications",
            " viewer: execute different display files",
            " builder: build simple panels"
        ]
    }

    DynamicCodeSlide {
        title: "CSDataQuick.Data Module"
        notes: '
        Data source: catest.EGU, calc 
        '
        code: '
import QtQuick 2.0
import QtQuick.Layouts 1.4

import CSDataQuick.Data 1.0

GridLayout {
    columns: 2

    CSData {
        id: data
        source: source.text
    }

    Text {
        text: "source"
    }
    TextInput {
        id: source
        Layout.fillWidth: true
        text: "catest"
    }

    Text {
        text: "value"
    }
    Text {
        text: data.value
        Layout.fillWidth: true
    }
}
        '
    }

    DynamicCodeSlide {
        title: "CSDataQuick.Components Module"
        notes: '
        Property names and Look-n-Feel taken from MEDM.
        Plot implmented using modified QCustomPlot for QtQuick.
        Edit in Qt Quick Designer.
        '
        code: '
import QtQuick 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import CSDataQuick.Components 1.0
ScrollView {
anchors.fill: parent
GridLayout {
    id: mainLayout
    width: parent.width
    columns: 3

    CSText {
        Layout.columnSpan: mainLayout.columns
        Layout.fillWidth: true
        Layout.preferredHeight: 30
        text: "Monitors"
        font.bold: true
        align: Text.AlignHCenter
        background: "lightBlue"
    }
    CSTextUpdate {
        Layout.alignment: Qt.AlignHCenter
        source: "catest"
        unitsVisible: true
    }

    CSByte {
        source: "calc"
    }

    CSBar {
        Layout.preferredHeight: 20
        source: "catest"
    }

    CSIndicator {
        Layout.preferredHeight: 20
        source: "catest"
    }

    CSText {
        Layout.columnSpan: mainLayout.columns
        Layout.fillWidth: true
        Layout.preferredHeight: 30
        text: "Graphics"
        font.bold: true
        align: Text.AlignHCenter
        background: "lightBlue"
    }
    
    CSArc {
        width: 50; height: 50
        begin: 30; span: 150
    }

    CSOval {
        width: 80; height: 40
    }

    CSPolyline {
        width: 50; height: 50
        points: [
            Qt.point(0, height/2),
            Qt.point(width/3, 0),
            Qt.point(2*width/3, 0),
            Qt.point(width, height/2),
            Qt.point(2*width/3, height),
            Qt.point(width/3, height),
            Qt.point(0, height/2)
        ]
        dynamicAttribute.channel: "bo"
        dynamicAttribute.visibilityMode: VisibilityMode.IfNotZero
    }

    CSPolygon {
        width: 50; height: 50
        points: [
            Qt.point(width/2, 0),
            Qt.point(0, height),
            Qt.point(width, height)
        ]
        dynamicAttribute.channel: "bo"
        dynamicAttribute.visibilityMode: VisibilityMode.IfZero
    }

    CSText {
        Layout.columnSpan: mainLayout.columns
        Layout.fillWidth: true
        Layout.preferredHeight: 30
        text: "Controls"
        align: Text.AlignHCenter
        font.bold: true
        background: "lightBlue"
    }

    CSTextEntry {
        Layout.fillWidth: true
        source: "wavec"
        format: TextFormat.String
    }

    CSMessageButton {
        text: "Reset Me!"
        source: "wavec"
        foreground: "red"
        offMessage: "\\x00"
    }

    CSTextArea {
        Layout.preferredWidth: 180
        Layout.rowSpan: 2
        source: "wavec"
    }

    CSSlider {
        source: "catest"
        stepSize: 0.1
        labelStyle: LabelStyle.Channel
    }

    CSWheelSwitch {
        source: "catest"
    }

    CSMenu {
        source: "calc.SCAN"
    }

    CSChoiceButton {
        stacking: Stacking.Row
        source: "bo"
    }

    CSText {
        Layout.columnSpan: mainLayout.columns
        Layout.fillWidth: true
        Layout.preferredHeight: 30
        text: "Plots"
        align: Text.AlignHCenter
        font.bold: true
        background: "lightBlue"
    }

    CSStripChart {
        Layout.columnSpan: mainLayout.columns
        Layout.fillWidth: true
        model: [
            { "channel": "catest" }
        ]
    }

    CSCartesianPlot {
        Layout.columnSpan: mainLayout.columns
        Layout.fillWidth: true
        model: [
            {
                "xchannel": "x",
                "ychannel": "y",
                "foreground": "black"
            }
        ]
    }
}
}
'
    }

    Slide {
        title: 'Integrate into other programs'
        notes: '
        Demo
        '
        content: [
            'QWidget based programs',
            ' Add QQuickWidget from Qt Designer or programatically',
            ' QQuickWidget::setSource(<qml_file>)',
            'PyQt',
            ' Register objects derived from QObject',
            ' Communicate via signal/slot',
            ' Create qml plugins'
        ]
    }

    Slide {
        title: 'Web browser'
        notes: '
        Qt has integrated WebKit/Chromium to WebEngine module.

        Ctrl+S show the code.

        Possible to communicate from within the browser to Qt via WebChannel.
        '
        SplitView {
            anchors.fill: parent
            SimpleWebview {
                id: webView
                Layout.minimumWidth: 500
                Layout.fillWidth: true
            }
            CodeSection {
                id: sourceCode
                text: readFile("SimpleWebview.qml")
                visible: false
            }
        }
        Keys.onPressed: {
            if (event.modifiers == Qt.MetaModifier && event.key == Qt.Key_S) {
                sourceCode.visible = !sourceCode.visible
            }
        }
    }

    Slide {
        title: 'Web application'
        notes: '
        HTML5 + CSS + JavaScript has been the norm of web development.
        '
        content: [
            'Client',
            ' Use XMLHttpRequest within QML',
            'WebGL platform',
            ' Make QtQuick applications webservers which renders contents in WebGL',
            ' Pass "-platform webgl:port=50001"',
            'WebAssembly/WASM',
            ' Sandboxed VM in web browsers',
            ' Supported as a target platform since Qt 5.12'
        ]
    }

    Slide {
        notes: '
        Let me finish up with The Qt Company\'s slogan
        '
        centeredText: "The Future is written with Qt"
    }

    Slide {
        writeInText: "Qt's future is in QtQuick"
    }

    Slide {
        centeredText: "By the way ..."
    }

    Slide {
        title: 'Source of this talk'
        contentWidth: width / 2
        content: [
            "Repository:",
            " https://github.com/xiaoqiangwang/CSDataQuick/doc/presentation",
            "Reference:",
            " Qml Book: https://qmlbook.github.io",
            " PyQt and Qml: https://www.riverbankcomputing.com/static/Docs/PyQt5/qml.html"
        ]
        CodeSection {
            text: readFile("EPICS_Collabration_Meeting_20210708.qml")
        }
    }

    function readFile(fileUrl) {
        var request = new XMLHttpRequest();
        request.open("GET", fileUrl, false);
        request.send(null);
        return request.responseText;
    }
}
