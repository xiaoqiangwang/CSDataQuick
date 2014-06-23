#include "viewer.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent)
{
    QAction *actionOpen = new QAction(tr("Open..."), this);
    actionOpen->setObjectName("actionOpen");
    QAction *actionExit = new QAction(tr("Exit"), this);
    actionExit->setObjectName("actionExit");
    QAction *actionAbout = new QAction(tr("About..."), this);
    actionAbout->setObjectName("actionAbout");

    QMenu *menuFile = new QMenu(tr("File"));
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionExit);
    menuBar()->addMenu(menuFile);

    QMenu *menuView = new QMenu(tr("View"));
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

void Viewer::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       tr("About ADL Viewer"),
                       tr("An ADL display file viewer in QML."));
}
