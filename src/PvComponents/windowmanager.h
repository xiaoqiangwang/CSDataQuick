#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QList>

class QWindow;
class QQuickCloseEvent;

typedef struct {
    QWindow * window;
    QString absFilePath;
    QString macro;
} WindowEntry;

class WindowManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> windows READ windows NOTIFY windowsChanged)
    Q_PROPERTY(QWindow * mainWindow MEMBER mMainWindow)
public:
    explicit WindowManager(QObject *parent = 0);

    Q_INVOKABLE void appendWindow(QWindow *window, QString absFilePath, QString macro);
    Q_INVOKABLE void removeWindow(QWindow *window);
    Q_INVOKABLE QWindow* findWindow(QString absFilePath, QString macro);
    Q_INVOKABLE void closeAllWindow();

    Q_INVOKABLE void closeWindow(QWindow *window);
    Q_INVOKABLE void printWindow(QWindow *window);

    Q_INVOKABLE void setMainWindow(QWindow *window) {mMainWindow = window;}

    QList<QObject*> windows();

signals:
    void windowsChanged();

public slots:
    void windowDestroyed();
    void onClosingWindow(QQuickCloseEvent *event);

protected:
    QList<QWindow*> mWindows;
    QWindow *mMainWindow;
};

#endif // WINDOWMANAGER_H
