#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QList>
#include <QUrl>

class QWindow;
class QQuickCloseEvent;

class WindowEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QWindow* window MEMBER window CONSTANT)
    Q_PROPERTY(QUrl filePath MEMBER filePath CONSTANT)
    Q_PROPERTY(QString macro MEMBER macro CONSTANT)
public:
    WindowEntry(QWindow *window, QUrl filePath, QString macro) {
        this->window = window;
        this->filePath = filePath;
        this->macro = macro;
    }

    QWindow * window;
    QUrl filePath;
    QString macro;
};

class WindowManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> entries READ entries NOTIFY entriesChanged)
    Q_PROPERTY(QWindow * mainWindow MEMBER mMainWindow)
public:
    explicit WindowManager(QObject *parent = 0);

    Q_INVOKABLE void appendWindow(QWindow *window, QUrl absFilePath, QString macro);
    Q_INVOKABLE QWindow* findWindow(QUrl absFilePath, QString macro);
    Q_INVOKABLE void closeAllWindow();

    Q_INVOKABLE void closeWindow(QWindow *window);
    Q_INVOKABLE void printWindow(QWindow *window);

    Q_INVOKABLE void setMainWindow(QWindow *window) {mMainWindow = window;}

    void removeWindow(QWindow *window);

    QList<QObject*> entries();

signals:
    void entriesChanged();

public slots:
    void windowDestroyed();
    void onClosingWindow(QQuickCloseEvent *event);

protected:
    QList<QWindow*> mWindows;
    QWindow *mMainWindow;
};

#endif // WINDOWMANAGER_H
