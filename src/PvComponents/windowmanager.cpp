/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "windowmanager.h"

#include <QWindow>
#include <QVariant>

#include <QtDebug>

bool windowFilePathCompare(const QWindow *w1, const QWindow *w2)
{
    return w1->filePath() < w2->filePath();
}

WindowManager::WindowManager(QObject *parent)
    : QObject(parent), mMainWindow(Q_NULLPTR)
{

}


void WindowManager::appendWindow(QWindow *window, QString absFilePath, QString macro)
{
    Q_UNUSED(absFilePath);
    Q_UNUSED(macro);

    mWindows.append(window);
    connect(window, SIGNAL(destroyed()), this, SLOT(windowDestroyed()));
    emit windowsChanged();
}

void WindowManager::windowDestroyed()
{
    QWindow * window = qobject_cast<QWindow*>(sender());
    qDebug() << window << "has been destroyed";
    if (window)
        removeWindow(window);
}

void WindowManager::removeWindow(QWindow *window)
{
    mWindows.removeOne(window);
    emit windowsChanged();
}

QWindow* WindowManager::findWindow(QString absFilePath, QString macro)
{
    QWindow *window = Q_NULLPTR;
    foreach (QWindow *w, mWindows) {
        if (w->filePath() == absFilePath &&
                w->property("macro") == macro) {
            window = w;
            break;
        }
    }
    return window;
}

void WindowManager::closeAllWindow()
{
    foreach (QWindow *w, mWindows) {
        w->close();
    }
}

QList<QObject*> WindowManager::windows()
{
    QList<QWindow*> sortedWindows(mWindows);
    qSort(sortedWindows.begin(), sortedWindows.end());

    QList<QObject*> windowsList;
    foreach (QWindow *w, sortedWindows) {
        windowsList.append(qobject_cast<QObject*>(w));
    }
    return windowsList;
}

