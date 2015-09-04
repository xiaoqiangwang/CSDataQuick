#ifndef VIEWER_H
#define VIEWER_H

#include <QMap>
#include <QMainWindow>
#include <QQmlEngine>

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

    void dispatchRequestReceived(QString fileName, QMap<QString, QString> macro, QRect geometry);
private:
    QQmlEngine engine;

};

#endif // VIEWER_H
