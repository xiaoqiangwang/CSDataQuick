#include <QApplication>

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>

class WebApp : public QMainWindow
{
    Q_OBJECT

public:
    WebApp(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

protected:



};

WebApp :: WebApp(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    QMdiArea *mdiArea = new QMdiArea();
    QMdiSubWindow *subWindow1 = new QMdiSubWindow;
    mdiArea->addSubWindow(subWindow1);
    subWindow1->setGeometry(50, 200, 300,300);
    setCentralWidget(mdiArea);

}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    WebApp win;
    win.showMaximized();
    win.show();

    return app.exec();
}

#include "main.moc"
