/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "windowmanager.h"

#include <QWindow>

WindowManager::WindowManager(QObject *parent)
    : QObject(parent)
{

}


void WindowManager::appendWindow(QWindow *window, QString absFilePath, QString macro)
{
    WindowEntry entry = {window, absFilePath, macro};
    mWindowsList.append(entry);
    connect(window, SIGNAL(destroyed()), this, SLOT(windowDestroyed()));
}

void WindowManager::windowDestroyed()
{
    QWindow * window = qobject_cast<QWindow*>(sender());
    if (window)
        removeWindow(window);
}

void WindowManager::removeWindow(QWindow *window)
{
    for(int i=0; i<mWindowsList.size(); i++) {
        if(mWindowsList.at(i).window == window) {
            mWindowsList.removeAt(i);
            break;
        }
    }
}

QWindow* WindowManager::findWindow(QString absFilePath, QString macro)
{
    QWindow *window = Q_NULLPTR;
    for(int i=0; i<mWindowsList.size(); i++) {
        if(mWindowsList.at(i).absFilePath == absFilePath
         &&mWindowsList.at(i).macro == macro) {
            window = mWindowsList.at(i).window;
        }
    }
    return window;
}

void WindowManager::closeAllWindow()
{
    foreach(WindowEntry entry, mWindowsList)
        entry.window->close();
}
