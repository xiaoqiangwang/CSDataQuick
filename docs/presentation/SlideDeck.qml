import QtQuick 2.0
import Qt.labs.presentation 1.0

Presentation
{
    id: presentation

    width: 1280
    height: 720

    SlideCounter {}
    Clock {}


    Slide {
        centeredText: "Create Dynamic GUI, <p><p>The <i><b>Quick</b></i> Way"
    }

    Slide {
        centeredText: "Hello World!"
    }

    Slide {
        title: "The Gtk Version"
        CodeSection {
            x: 0
            width: parent.width
            text: "
#include <gtk/gtk.h>
 
int main (int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *label;
 
    gtk_init(&argc, &argv);
 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 
    gtk_window_set_title(GTK_WINDOW(window), \"Hello World!\");
 
    g_signal_connect(window, \"destroy\", G_CALLBACK(gtk_main_quit), NULL);
 
    label = gtk_label_new(\"Hello World!\");
 
    gtk_container_add(GTK_CONTAINER(window), label);
 
    gtk_widget_show_all(window);
 
    gtk_main();
 
    return 0;
}
"
        }
    }
    
    Slide {
        title: "The Swing Version"
        CodeSection {
            x: 0
            width: parent.width
            text: "
import javax.swing.*;        

public class HelloWorldSwing {
    public static void main(String[] args) {
        JFrame frame = new JFrame(\"HelloWorldSwing\");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JLabel label = new JLabel(\"Hello World!\");
        frame.getContentPane().add(label);

        frame.pack();
        frame.setVisible(true);
    }
}
"

        }
    }
    Slide {
        title: "The Qt Version"
        CodeSection {
            x: 0
            width: parent.width
            text: "
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>

int main(int argc, char **argv)
{
    QApplication app(argv);
    QLabel label(\"Hello World!\");
    label.show();
    app.exec();
}
"
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
        title: "QtQuick"
        content: [
            "QtQuick: Declarative way of building dynamic GUI",
            "  QML: Qt Meta Language",
            "    Javascript alike, JIT optimized",
            "    Extensible with C++",
            "  Scene graph based rendering. Non-imperative.",
        ]
    }

    Slide {
        title: "Animation"
    }

    Slide {
        title: "WebKit"
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
            "Repo: http://bitbucket.org/xwang/epicsqml"
        ]
        CodeSection {
            text: "import QtQuick 2.0\n" +
                  "import Qt.labs.presentation 1.0\n" +
                  "\n" +
                  "Presentation {\n" +
                  "    id: presentation\n" +
                  "\n" +    
                  "    width: 1280\n" +
                  "    height: 720\n" +
                  "\n" +
                  "SlideCounter {}\n" +
                  "Clock {}\n" +
                  "\n" +
                  "Slide {\n" +
                  "    centeredText: \"Create Dynamic GUI, Quick\"\n" +
                  "}\n" +
                  "Slide {\n" +
                  "    centeredText: \"Hello World!\"\n" +
                  "}\n" +
                  "\n" +
                  "Slide {\n" +
                  "title: \"The Gtk Version\"\n" +
                  "CodeSection {\n" +
                  "    x: 0\n" +
                  "    width: parent.width\n" +
                  "    text: \"\n" +
                  "\n" +
                  "#include <gtk/gtk.h>\n"
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

    CodeSlide {
        title: "CodeSlide {} Element"
        code:
"CodeSlide {
    title: \"CodeSlide {} Element\"
    code:
\"
// Whitespaces are preserved,
// so we start at the beginning of the line...

// You can mouse click on any line

// Navigate with keypad when the code has focus

int main(int argc, char **argv) {
    QGuiApplication app;
    QWindow window;
    window.show();
    return app.exec();
}
\" "
    }

    Slide {
        title: "Font size relative to screen size"
        content: [
            "Which means you don't need to worry about it",
            "Bullet points wrap around on the edges, regardless of how long they are, like this. Even if you should choose to use a very long bullet point (which would distract your audience) it would still look ok'ish",
            "If you run out of height, you're out of luck though..."
        ]
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
        title: "Features"
        centeredText: 'Hit [C] to fade out the current page if there are questions from the audience'
    }

    Slide {
        title: "Features"
        centeredText: 'Navigate back and forth using [left] and [right]\n[space] or [click] takes you to the next slide.'
    }

    CodeSlide {
        title: "Slide Counter"
        code:
"Presentation {

    SlideCounter {
        // Defaults:
        // anchors.right: parent.right
        // anchors.bottom: parent.bottom
        // anchors.margins: fontSize;
        // textColor: 'black'
        // fontFamily: 'Helvetica' (from presentation)
        // fontScale: 0.5;
        }
    }

    Slide {
        ...
    }
}"
    }

    CodeSlide {
        title: "Clock"
        code:
"Presentation {

    Clock {
        // Defaults:
        // anchors.let: parent.left
        // anchors.bottom: parent.bottom
        // anchors.margins: fontSize;
        // textColor: 'black'
        // fontFamily: 'Helvetica' (from presentation)
        // fontScale: 0.5;
        }
    }

    Slide {
        ...
    }
}"
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


}
