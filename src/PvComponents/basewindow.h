#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QQuickWindow>

class BaseWindow : public QQuickWindow
{
    Q_OBJECT

    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath)

public:
    explicit BaseWindow(QWindow *parent=0);
    virtual ~BaseWindow();

    Q_INVOKABLE void printWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Q_DISABLE_COPY(BaseWindow)
};

#endif
