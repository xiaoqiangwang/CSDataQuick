/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "basewindow.h"

#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>

BaseWindow :: BaseWindow(QWindow *parent)
    : QQuickWindow(parent)
{
}

BaseWindow :: ~BaseWindow()
{
}

void BaseWindow :: closeEvent(QCloseEvent *event)
{
    event->accept();
    deleteLater();
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
