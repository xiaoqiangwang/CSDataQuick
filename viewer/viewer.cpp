#include "viewer.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QDir>
#include <QApplication>

#include <QQmlComponent>
#include <QQuickWindow>
#include <QPlainTextEdit>
#include <QDateTime>

#include <QtDebug>

#include <fstream>
#include <sstream>

#include "common.h"
#include "element.h"
#include "fileinfo.h"

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(QApplication::applicationName()  + " " + QApplication::applicationVersion());
    engine.addImportPath("..");

    QAction *actionOpen = new QAction(tr("Open..."), this);
    actionOpen->setObjectName("actionOpen");
    QAction *actionExit = new QAction(tr("Exit"), this);
    actionExit->setObjectName("actionExit");
    QAction *actionDisplayList = new QAction(tr("Display List"), this);
    actionDisplayList->setObjectName("actionDisplayList");
    QAction *actionAbout = new QAction(tr("About..."), this);
    actionAbout->setObjectName("actionAbout");

    QMenu *menuFile = new QMenu(tr("File"));
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionExit);
    menuBar()->addMenu(menuFile);

    QMenu *menuView = new QMenu(tr("View"));
    menuView->addAction(actionDisplayList);
    menuBar()->addMenu(menuView);

    QMenu *menuHelp = new QMenu(tr("Help"));
    menuHelp->addAction(actionAbout);
    menuBar()->addMenu(menuHelp);

    editor = new QPlainTextEdit(this);
    editor->setReadOnly(true);
    setCentralWidget(editor);

    connect(this, SIGNAL(sendMessage(QString)), editor, SLOT(appendPlainText(QString)));

    QMetaObject::connectSlotsByName(this);
}

void Viewer::closeEvent(QCloseEvent *event)
{
    /* Close all adl windows and quit applicaiton */
    foreach (QQuickWindow *window, windows) {
        window->close();
    }
    qApp->quit();
}

void Viewer::on_actionOpen_triggered()
{
}

void Viewer::on_actionExit_triggered()
{
    close();
}

void Viewer::on_actionDisplayList_triggered()
{
}

void Viewer::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       tr("About ADL Viewer"),
                       tr("<h2>ADL Viewer %1</h2>\n\n"
                          "Based on Qt %2\n")
                       .arg(QApplication::applicationVersion())
                       .arg(QT_VERSION_STR)
                       );
}

void Viewer::dispatchRequestReceived(QString fileName, QMap<QString, QString> macroMap, QRect geometry)
{
    qDebug() << "File Dispatch Request:\n"
             << "  fileName =" << fileName << "\n"
             << "  macro =" << macroMap << "\n"
             << "  geometry =" << geometry;

    openADLDisplay(fileName, macroMap, geometry);
}

void Viewer :: outputMessage(QtMsgType type, const QString &msg)
{
    emit sendMessage(QDateTime::currentDateTime().toString() + ":\n" + msg);
}

void Viewer :: childWindowClosed(QQuickCloseEvent *event)
{
    Q_UNUSED(event);

    QQuickWindow *window = qobject_cast<QQuickWindow *>(sender());
    if (window) {
        windows.removeOne(window);
    }
}


void Viewer :: openADLDisplay(QString fileName, QMap<QString, QString> macroMap, QRect geometry)
{

    QFile file;
    if (QDir::isAbsolutePath(fileName))
        file.setFileName(fileName);
    else
        file.setFileName("displays://" + fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open " << fileName;
        return;
    }
    std::string input(file.readAll().data());
    std::istringstream isstream(input);

    Display displayInfo(0);
    displayInfo.setFileName(file.fileName().toStdString());
    std::map<std::string, std::string> macros;
    foreach (QString macro, macroMap.keys()) {
        macros[macro.toStdString()] = macroMap[macro].toStdString();
    }
    displayInfo.setMacros(macros);
    displayInfo.parse(isstream);

    std::ostringstream osstream;
    displayInfo.toQML(osstream);
    osstream.flush();

    QQmlComponent component(&engine);
    component.setData(osstream.str().c_str(), QUrl());
    while(!component.isReady()) {
        if (component.isError()) {
            foreach(QQmlError error, component.errors())
                qDebug() << error;
            return;
        }
    }
    QQuickWindow * window = qobject_cast<QQuickWindow *>(component.create());
    connect(window, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(childWindowClosed(QQuickCloseEvent*)));
    windows.append(window);

    window->setTitle(fileName);
    window->show();
}
