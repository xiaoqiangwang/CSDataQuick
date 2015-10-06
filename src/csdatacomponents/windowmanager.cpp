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

/*!
    \qmltype WindowManager
    \inqmlmodule PvComponents
    \brief Singlton object to manage all opened windows.

    Example:
    \qml
        ListView {
            model: WindowManager.entries
            delegate: Text {
                text: model.modelData.filePath
            }
        }
    \endqml
*/
bool windowFilePathCompare(const QWindow *w1, const QWindow *w2)
{
    return w1->filePath() < w2->filePath();
}

WindowManager::WindowManager(QObject *parent)
    : QObject(parent), mMainWindow(Q_NULLPTR)
{

}
/*!
    \qmlmethod WindowManager::appendWindow(QWindow *window, QUrl absFilePath, QString macro)

    Append an opened window instance \a window, created with \a absFilePath and \a macro.
 */
void WindowManager::appendWindow(QWindow *window, QUrl absFilePath, QString macro)
{
    Q_UNUSED(absFilePath);
    Q_UNUSED(macro);

    mWindows.append(window);
    connect(window, SIGNAL(destroyed()), this, SLOT(windowDestroyed()));

    QQuickWindow * qwindow = qobject_cast<QQuickWindow*>(window);
    if (qwindow)
        connect(qwindow, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(onClosingWindow(QQuickCloseEvent*)));

    emit entriesChanged();
}

/*!
    \qmlmethod WindowManager::closeWindow(QWindow *window)

    Close and delete a window instance \a window.
 */
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
    emit entriesChanged();
}

/*!
    \qmlmethod WindowManager::findWindow(QUrl absFilePath, QString macro)

    Search for a window instance opened with \a absFilePath and \a macro.
    It will return the found instance or null otherwise.
 */
QWindow* WindowManager::findWindow(QUrl absFilePath, QString macro)
{
    QWindow *window = Q_NULLPTR;
    foreach (QWindow *w, mWindows) {
        if (w->property("filePath") == absFilePath &&
                w->property("macro") == macro) {
            window = w;
            break;
        }
    }
    return window;
}

/*!
    \qmlmethod WindowManager::printWindow(QWindow *window)
    \brief Grab a screenshot of \a window and send to printer.

    The actual printing is performed by Qt Print Support.

    \sa QQuickWindow::grabWindow
 */
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

/*!
    \qmlmethod WindowManager::closeAllWindow()

    Close and delete all opened windows.
 */
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

/*!
    \qmlproperty list WindowManager::entries

    The list of all opened windows. Each entry has the follow field,
    \list
    \li filePath - absolute file path this window represents
    \li macro - macro expansion used to create this window
    \li window - the window instance
    \endlist
 */
QList<QObject*> WindowManager::entries()
{
    // sort windows by filePath, so that the filePath can be used as section header in ListView
    QList<QWindow*> sortedWindows(mWindows);
    qSort(sortedWindows.begin(), sortedWindows.end(), windowFilePathCompare);

    QList<QObject*> entries;
    foreach(QWindow *w, sortedWindows) {
        QUrl absFilePath = w->property("filePath").toUrl();
        QString macro = w->property("macro").toString();
        entries.append(qobject_cast<QObject*>(new WindowEntry(w, absFilePath, macro)));
    }
    return entries;
}
