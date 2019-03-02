/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "basewindow.h"

#include <QFileInfo>
#include <QPainter>
#include <QPrintDialog>
#include <QQuickItem>

#include <QtDebug>

#ifndef NO_PRINTERSUPPORT
#include <QPrinter>
#endif

BaseWindow :: BaseWindow(QWindow *parent)
    : QQuickWindow(parent)
{
    connect(this, SIGNAL(closing(QQuickCloseEvent*)), SLOT(deleteLater()));
}

BaseWindow :: ~BaseWindow()
{
}

QString BaseWindow :: filePath() const
{
    return QQuickWindow::filePath();
}
void BaseWindow :: setFilePath(const QString &filePath)
{
    QQuickWindow::setFilePath(filePath);
    emit filePathChanged();

    // set file name as window title
    QFileInfo fi(filePath);
    setTitle(fi.fileName());
}

QString BaseWindow :: macro() const
{
    return _macro;
}
void BaseWindow :: setMacro(const QString &macro)
{
    if (_macro != macro) {
        _macro = macro;
        emit macroChanged();
    }
}

void BaseWindow :: printWindow()
{
    QImage snapshot = grabWindow();
#ifndef NO_PRINTERSUPPORT
    QPrinter printer;
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.drawImage(0, 0, snapshot);
    }
#endif
}

void BaseWindow :: resizeEvent(QResizeEvent *event)
{
    QQuickItem *root = contentItem();
    if (!root)
        return;

    qreal rx = 1.0 * event->size().width() / event->oldSize().width();
    qreal ry = 1.0 * event->size().height() / event->oldSize().height();

    foreach(QQuickItem *child, root->childItems()) {
        child->setPosition(QPointF(child->x() * rx, child->y() * ry));
        child->setSize(QSizeF(child->width() * rx, child->height() * ry));
    }
}
