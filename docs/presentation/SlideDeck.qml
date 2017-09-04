import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import CSDataQuick.Components 1.0

Presentation
{
    id: presentation

    width: 1280
    height: 720

    showNotes: true

    SlideCounter {}
    Clock {}

    Slide {
        id: master
        anchors.fill: parent
        Image {
            anchors.top: parent.top
            source: "PSI Aerial.png"
            width: parent.width
            height: parent.height / 2
        }
        Text {
            width: parent.width
            height: parent.height / 2
            anchors.top: parent.verticalCenter
            anchors.topMargin: master.fontSize * 1.5
            horizontalAlignment: Text.Center
            font.pixelSize: master.baseFontSize
            font.family: fontFamily
            color: textColor
            wrapMode: Text.Wrap

            text:"QtQuick/Qml, and EPICS<p><p>Dynamic GUI in the <i><b>Quick</b></i> Way"
        }
        notes: '
        You are all familiar with Qt since the universal introduction of caqtdm.

        Today I am going to show another part of Qt, which is quite different from the classic Qt.
        '
    }

    Slide {
        title: 'History and Now'
        
        notes: '
        However they failed one by one. It seems impossible to gain a share of the market ruled by Android and IOS.
        But as a technology, QtQuick survived. It is still a choice for cross platform mobile development.
        It gets expanded into embedded system as well.

        I cannot say much about desktop applications. But KDE shell has migrated to QtQuick.

        '
        content: [
        "aka Qt Declarative",
        "QtQuick 1 in Qt 4.7, 2010 - QtQuick 2.7 in Qt 5.9, 2017",
        "Targeted towards mobile development at the era of Nokia", 
        "Followed by Ubuntu Touch and BlackBerry 10",
        "Mobile app development for IOS and Android",
        "Embedded system, especially in-car control system", 
        "Desktop applications, KDE Plasma 5"
        ]
    }

    Slide {
        centeredText: "Hello World!"

        notes: '
        This talk is to give you a flavor through simple examples.

        And no example is simpler than hello world. 
        
        Before showing the QtQuick version, let us see how to program a hello world example in other toolkits.
        '
    }

    Slide {
        title: "The Gtk Version"
        notes: '
        I am not a GTK user so this example is taken from its tutorial.
        Even not an expert, we can see it first creates a window, 
        and exit when this window gets closed. The text label is then created inside
        the window. The last step is to show the window and enter into the
        event loop.
        That is quite a standard launching sequence for a GUI program.
        '
        CodeSection {
            x: 0
            width: parent.width
            text: '
#include <gtk/gtk.h>
 
int main (int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *label;
 
    gtk_init(&argc, &argv);
 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 
    gtk_window_set_title(GTK_WINDOW(window), "Hello World!");
 
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
 
    label = gtk_label_new("Hello World!");
 
    gtk_container_add(GTK_CONTAINER(window), label);
 
    gtk_widget_show_all(window);
 
    gtk_main();
 
    return 0;
}
'
        }

    }
    
    Slide {
        title: "The Qt Widget Version"
        CodeSection {
            x: 0
            width: parent.width
            text: '
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>

int main(int argc, char **argv)
{
    QApplication app(argv);
    QLabel label("Hello World!");
    label.show();
    app.exec();
}
'
        }
        notes: '
        Next it is the Qt verison. It is already simpler. QApplication does the right thing
        to create the toplevel window and quit when the window is closed.
        '
    }
   
    Slide {
        writeInText: 'Can it be shorter?'
    }

    DynamicCodeSlide {
        title: "The Quick Version"
        notes: '
        The quick version is remarkably different. The import statement looks like many other languages.
        It brings components from other modules into the namespace.
        
        The creation of the a text lable is nothing like previouse examples. Syntatically it is a structural text,
        like a literal object in JavaScript. This object has a named type, and properties to describe it.

        The text property holds the greeting string to display. We can control its appearance via other properties assignment.
        Color, Font, Style
        '
        code: '
import QtQuick 2.5

Text {
    text: "Hello World!"
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
            " The standard library, visual items, animations, models/views",
            " Scene Graph with multiple render backends",
        ]

        notes:'
        QML propabably stands for Qt Model Language, Qt Markup Language.
        
        OpenGL was the only render backend until Qt 5.6. Software render was added in case of bad OpenGL driver.
        In Qt 5.10 Vulkan will be added, the successor to OpenGL.

        Now I will intruduce some key aspect  through examples.
        '
    }

    DynamicCodeSlide {
        title: "Property Binding"
        notes: '
        This is the core feature of QML. It lets you specify relationship between different properties
        from different objects.

        Here the blue box top left is set to track the red box bottom right. It moves wherever the red
        box goes.
        '
        readOnly: true
        code: '
import QtQuick 2.5

Item {
    anchors.fill: parent

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
        x: box_red.x + 80
        y: box_red.y + 80
        width: 80
        height: 80
        color: "blue"
    }
}
        '
    }

    DynamicCodeSlide {
        title: "States/Transition/Animation"
        notes: '
        A state is set of property settings. The intial default state is defined by the assigned property values.
        New states can be created by grouping different property changes.
        Here we change the text color and font size.

        To smooth the property changes between states, animations might be configured.
        Here the font size changes in half a second and color changes in 1 second.

        Together this creates a fluid user experience.
        '
        code: '
import QtQuick 2.5

TextInput {
    id: helloText
    text: "Hello World!"

    states: State {
        when: helloText.focus
        PropertyChanges { target: helloText; font.pixelSize: 30 }
        PropertyChanges { target: helloText; color: "red"}
    }
    transitions: Transition {
        NumberAnimation {duration: 500}
        ColorAnimation {duration: 1000}
    }
}
        '
    }

    Slide {
        title: "Web"
        notes: '
        Qt has since long integrated WebKit. Writing a simple browser cannot be easier.
        It is as simple as setting the url property to a WebView item.

        It is possible to communicate from within the browser to Qt via WebSocket.
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
            if (event.key == Qt.Key_S) {
                sourceCode.visible = !sourceCode.visible
            }
        }
    }

    DynamicCodeSlide {
        title: "Model/View/Delegate"
        notes: '
        Most toolkits use Model/View design pattern to separate data and its representation.
        Qt uses the  term Model/View/Delegate.
        
        Model contains the data. In this case it is a list of fruits. Each fruit gets properties
        like name and color.
        View organises the data display. Here it is shown in a column.
        Delegate decides how the data appears in the view. Here it simply displays the fruit name.

        But we can give it some color too.
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
        That all looks good. But how that integrates with our control system?
        '
    }

    Slide {
        title: "CSData"
        content: [
            "Properties: source, value, fieldType, alarm, units, timeStamp, precision, range, accessRight",
            "Methods: setValue",
            "Multiple source scheme, ca://, sim://",
            "Plugin based CSDataEngine",
        ]
        notes: '
        In EPICS the access unity is process variable. PV is a piece of named data with describing metadata.
        I have taken a data-centric approach to represent PV with CSData.
        '
    }

    Slide {
        title: "Filter Wheel"
        notes: '
        Hard x-ray beamlines often insert filters to attenuate the beam. In some cases, foils of different materials
        and thickness are mounted on a wheel selector.

        We can connect the mouse selection to change the PV value, and display the rotation process.
        '
        SplitView {
            anchors.fill: parent
            FilterWheel {
                Layout.minimumWidth: 300
                Layout.fillWidth: true
            }
            CodeSection {
                id: sourceCodeFW
                text: readFile("FilterWheel.qml")
            }
        }
    }

    Slide {
        title: "CSDataComponents"
        notes: '
        Of course there is a collection of standard items for display and control PVs.
        '
        GridLayout {
            columns: 3
        CSSlider {
            source: 'catest'
            stepSize: 0.1
            labelStyle: LabelStyle.Channel
        }

        CSMessageButton {
            text: 'Reset Me!'
            source: 'catest'
            foreground: 'yellow'
            offMessage: "0"
        }

        CSMenu {
            source: 'calc.SCAN'
        }

        CSByte {
            source: 'calc'
        }

        CSChoiceButton {
            stacking: Stacking.Row
            source: 'bo'
        }

        CSStripChart {
            model: ListModel {
                ListElement {
                    channel: 'catest'
                }
            }
        }
        CSCartesianPlot {
            model: ListModel{
                ListElement {
                    xchannel: "x"
                    ychannel: "y"
                    foreground: "black"
                }
            }
        }
        }
    }

    Slide {
        title: "Interested?"

        content: [
            "Compiled module: /afs/psi.ch/user/w/wang_x1/public/CSDataQuick-cmake/",
            "Documents: docs/html/csdataquick-overview.html"
        ]
    }


    Slide {
        centeredText: "By the way ..."
    }

    Slide {
        title: 'Source of this talk'
        contentWidth: width / 2
        content: [
            "Repository: https://github.com/xiaoqiangwang/CSDataQuick",
            "Reference: https://qmlbook.github.io/"
        ]
        CodeSection {
            text: readFile("SlideDeck.qml")
        }
    }

    function readFile(fileUrl) {
        var request = new XMLHttpRequest();
        request.open("GET", fileUrl, false);
        request.send(null);
        return request.responseText;
    }
}
