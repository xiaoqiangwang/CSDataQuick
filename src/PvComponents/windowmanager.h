#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QList>

class QWindow;

typedef struct {
    QWindow * window;
    QString absFilePath;
    QString macro;
} WindowEntry;

class WindowManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> windows READ windows NOTIFY windowsChanged)
public:
    explicit WindowManager(QObject *parent = 0);

    Q_INVOKABLE void appendWindow(QWindow *window, QString absFilePath, QString macro);
    Q_INVOKABLE void removeWindow(QWindow *window);
    Q_INVOKABLE QWindow* findWindow(QString absFilePath, QString macro);
    Q_INVOKABLE void closeAllWindow();

    QList<QObject*> windows();

signals:
    void windowsChanged();

public slots:
    void windowDestroyed();

protected:
    QList<QWindow*> mWindows;
};

#endif // WINDOWMANAGER_H
