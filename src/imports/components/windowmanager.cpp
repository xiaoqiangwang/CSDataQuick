/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
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
    return w1->property("filePath").toString() <= w2->property("filePath").toString();
}

WindowListModel::WindowListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> WindowListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(WindowRole, "window");
    roles.insert(PathRole, "filePath");
    roles.insert(MacroRole, "macro");
    return roles;
}

int WindowListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return windows.count();
}

QVariant WindowListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= windows.count())
        return QVariant();

    QWindow *window = windows.at(index.row());

    switch (role) {
    case WindowRole:
    {
        QVariant v;
        v.setValue(window);
        return v;
    }
    case PathRole:
        return window->property("filePath").toUrl();
    case MacroRole:
        return window->property("macro").toString();
    default:
        return QVariant();
    }
}

void WindowListModel::add(QWindow *window)
{
    if (windows.contains(window))
        return;
    auto it = qUpperBound(windows.begin(), windows.end(), window, windowFilePathCompare);
    beginInsertRows(QModelIndex(), it-windows.begin(), it-windows.begin());
    windows.insert(it, window);
    endInsertRows();
}

void WindowListModel::remove(QWindow *window)
{
    int i = windows.indexOf(window);
    beginRemoveRows(QModelIndex(), i, i);
    windows.removeAt(i);
    endRemoveRows();
}

QWindow* WindowListModel::find(QUrl absFilePath, QString macro) const
{
    QWindow *window = Q_NULLPTR;
    foreach (QWindow *w, windows) {
        if (w->property("filePath") == absFilePath &&
                w->property("macro") == macro) {
            window = w;
            break;
        }
    }
    return window;
}

bool WindowListModel::contains(QWindow *window) const
{
    return windows.contains(window);
}

/*!
    \qmltype WindowManager
    \inqmlmodule CSDataQuick.Components
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
WindowManager::WindowManager(QObject *parent)
    : QObject(parent), mMainWindow(Q_NULLPTR)
{
    model = new WindowListModel();
}
/*!
    \qmlmethod WindowManager::appendWindow(window)

    Append an opened window instance \a window.
 */
void WindowManager::appendWindow(QWindow *window)
{
    if (!window || model->contains(window))
        return;

    model->add(window);
    connect(window, SIGNAL(destroyed()), this, SLOT(windowDestroyed()));

    QQuickWindow * qwindow = qobject_cast<QQuickWindow*>(window);
    if (qwindow)
        connect(qwindow, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(onClosingWindow(QQuickCloseEvent*)));
}

/*!
    \qmlmethod WindowManager::closeWindow(window)

    Close and delete a window instance \a window.
 */
void WindowManager::closeWindow(QWindow *window)
{
    if (!window) {
        qWarning() << "Invalid window to close";
        return;
    }
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
    model->remove(window);
}

/*!
    \qmlmethod Window WindowManager::findWindow(absFilePath, macro)

    Search for a window instance opened with \a absFilePath and \a macro.
    It will return the found instance or null otherwise.
 */
QWindow* WindowManager::findWindow(QUrl absFilePath, QString macro)
{
    return model->find(absFilePath, macro);
}

/*!
    \qmlmethod WindowManager::printWindow(window)
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
        QPrintDialog printDialog(&printer);
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
    for(int i=0; i<model->rowCount(QModelIndex()); i++) {
        QVariant v = model->data(model->index(i), WindowRole);
        QWindow *w = v.value<QWindow*>();
        if (w) {
            w->close();
            w->deleteLater();
        }
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
    \qmlproperty ListModel WindowManager::entries

    The list of all opened windows. Each list element has the follow properties,
    \list
    \li filePath - absolute file path this window represents
    \li macro - macro expansion used to create this window
    \li window - the window instance
    \endlist
 */
QAbstractListModel* WindowManager::entries()
{
    return model;
}
