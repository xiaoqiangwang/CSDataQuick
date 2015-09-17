#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QQuickWindow>

class BaseWindow : public QQuickWindow
{
    Q_OBJECT

    Q_PROPERTY(QString fileName MEMBER mFileName NOTIFY fileNameChanged)

public:
    explicit BaseWindow(QWindow *parent=0);
    virtual ~BaseWindow();

    Q_INVOKABLE void printWindow();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void fileNameChanged();

private:
    Q_DISABLE_COPY(BaseWindow)

    QString mFileName;
};

#endif
