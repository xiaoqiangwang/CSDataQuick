/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "windowmanager.h"

#include <QWindow>
#include <QQuickWindow>
#include <QVariant>

#ifndef NO_PRINTERSUPPORT
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#endif

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

    QQuickWindow * qwindow = qobject_cast<QQuickWindow*>(window);
    if (qwindow)
        connect(qwindow, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(onClosingWindow(QQuickCloseEvent*)));

    emit windowsChanged();
}

void WindowManager::closeWindow(QWindow *window)
{
    window->close();
    window->deleteLater();
}

void WindowManager::windowDestroyed()
{
    QWindow * window = qobject_cast<QWindow*>(sender());
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

void WindowManager::printWindow(QWindow *window)
{
#ifndef NO_PRINTERSUPPORT
    QQuickWindow * qwindow = qobject_cast<QQuickWindow*>(window);
    if (qwindow) {
        QImage snapshot = qwindow->grabWindow();
        QPrinter printer;
        QPrintDialog printDialog(&printer, 0);
        if (printDialog.exec() == QDialog::Accepted) {
            QPainter painter(&printer);
            painter.drawImage(0, 0, snapshot);
        }
    }
#endif
}

void WindowManager::closeAllWindow()
{
    foreach (QWindow *w, mWindows) {
        w->close();
        w->deleteLater();
    }
}

void WindowManager::onClosingWindow(QQuickCloseEvent *event)
{
    // QQuickCloseEvent is actually not in public API
    // and only exposed in qquickwindow.h as of Qt 5.4
    Q_UNUSED(event)

    QWindow * window = qobject_cast<QWindow*>(sender());
    if (window)
        window->deleteLater();
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

