#ifndef VIEWER_H
#define VIEWER_H

#include <QMap>
#include <QVector>
#include <QMainWindow>
#include <QQmlEngine>

class QQuickWindow;
class QQuickCloseEvent;

class Viewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = 0);

signals:

public slots:
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionDisplayList_triggered();
    void on_actionAbout_triggered();

    void childWindowClosed(QQuickCloseEvent *event);
    void dispatchRequestReceived(QString fileName, QMap<QString, QString> macro, QRect geometry);

protected:
    void closeEvent(QCloseEvent *event);

private:
    QQmlEngine engine;
    QVector<QQuickWindow *> windows;
};

#endif // VIEWER_H
