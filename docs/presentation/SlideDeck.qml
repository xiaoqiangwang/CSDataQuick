import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import Qt.labs.presentation 1.0

Presentation
{
    id: presentation

    width: 1280
    height: 720

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
    }
    Slide {
        centeredText: "Hello World!"
    }

    Slide {
        title: "The Gtk Version"
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
    }
   
    Slide {
        writeInText: 'Can it be shorter?'
    }

    Slide {
        title: "The Quick Version"
        CodeSection {
            x: 0
            width: parent.width
            height: parent.height / 2
            text: "
import QtQuick 2.1

Text {
    text: 'Hello World!'
}
"
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.height * 3 / 4
            text: 'Hello World!'
            font.pointSize: 20
        }
    }

    Slide {
        title: "QtQuick/QML"
        content: [
            "QML",
            " The language and the infrastructure engine",
            " Declarative, JSON-like syntax",
            " Javascript expression for dynamic property binding",
            " Extensible with C++",
            "QtQuick",
            " The standard library, visual items, animations, models/views",
            " Scene graph based rendering with multiple backends",
        ]
    }

    Slide {
        title: "Animation"
    }

    Slide {
        title: "Web"
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

    Slide {
        title: "MVC"
    }

    Slide {
        centeredText: "E P I C S ?"
    }

    Slide {
        title: "Double Slit"

    }

    Slide {
        centeredText: "One last thing..."
    }

    Slide {
        title: 'Source of this talk'
        contentWidth: width / 2
        content: [
            "Repo: https://github.com/xiaoqiangwang/CSDataQuick"
        ]
        CodeSection {
            text: readFile("SlideDeck.qml")
        }
    }

    Slide {
        id: fillAreaSlide
        title: "Slide {}, continued"
        content: ["The built-in property \"contentWidth\" can be used to let the bullet points fill a smaller area of the slide..."]

        SequentialAnimation on contentWidth {
            PropertyAction { value: fillAreaSlide.width }
            PauseAnimation { duration: 2500 }
            NumberAnimation { to: fillAreaSlide.width / 2; duration: 5000; easing.type: Easing.InOutCubic }
            running: fillAreaSlide.visible
        }

        Rectangle {
            height: parent.height
            width: parent.contentWidth

            color: "lightGray"
            z: -1
        }
    }


    Slide {
        id: interactiveSlide

        title: "Embed Interactive Content"

        Rectangle {
            id: box
            width: parent.fontSize * 10
            height: width
            color: mouse.pressed ? "lightsteelblue" : "steelblue"

            NumberAnimation on rotation { from: 0; to: 360; duration: 10000; loops: Animation.Infinite; running: visible }

            Text {
                text: "Click Me!"
                anchors.centerIn: parent
            }

            MouseArea {
                id: mouse
                anchors.fill: parent
                drag.target: box
            }
        }
    }


    Slide {
        title: "Customizations"
        titleColor: "white"

        textColor: "green"
        fontFamily: "Times New Roman"

        Rectangle {
            x: -parent.x
            y: -parent.y
            width: presentation.width
            height: parent.y * 0.9
            color: "black"
        }

        content: [
            "Bullet points, centered text, write-in-text or code listings, can be changed using 'textColor'",
            "Title can be changed with 'textColor'",
            "Font can be changed using 'fontFamily'",
            "Change 'fontScale' for bigger or smaller fonts",
            "All can be set globally on the presentation (and then inherited in all Slide {} elements), or set directly on a specific Slide {} element."
        ]
    }

    CodeSlide {
        title: "Slide Notes in Another Window"
        code:
"Presentation {
    showNotes: true;

    Slide {
        title: 'Slide One'
        content: ['bullet point', 'bullet point'];
        notes: 'Here cometh the notes...'
    }

    ...

    // Check out examples/notes for a running example
}
"
    }


    Slide {
        centeredText: "Now go make our own presentations\n\nEnjoy!"
    }

    function readFile(fileUrl) {
        var request = new XMLHttpRequest();
        request.open("GET", fileUrl, false);
        request.send(null);
        return request.responseText;
    }
}
