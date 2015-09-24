/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "basewindow.h"

#include <QFileInfo>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>

#include <QtDebug>

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
    QPrinter printer;
    QPrintDialog printDialog(&printer, 0);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.drawImage(0, 0, snapshot);
    }
}
