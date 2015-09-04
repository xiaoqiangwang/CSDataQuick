#include "viewer.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QDir>

#include <QQmlComponent>
#include <QQuickView>

#include <QtDebug>

#include <fstream>
#include <sstream>

#include "common.h"
#include "element.h"
#include "fileinfo.h"

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent)
{
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

    QMetaObject::connectSlotsByName(this);
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
                       tr("An ADL display file viewer in QML."));
}

void Viewer::dispatchRequestReceived(QString fileName, QMap<QString, QString> macroMap, QRect geometry)
{
    qDebug() << "File Dispatch Request:"
             << "  fileName =" << fileName
             << "  macro =" << macroMap
             << "  geometry =" << geometry;

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
    QWindow * window = qobject_cast<QQuickWindow *>(component.create());
    window->show();
}
