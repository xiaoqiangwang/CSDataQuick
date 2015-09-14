#include <basewindow.h>

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
