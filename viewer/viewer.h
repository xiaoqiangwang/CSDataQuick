#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>

class Viewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = 0);

signals:

public slots:
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();

private:
};

#endif // VIEWER_H
