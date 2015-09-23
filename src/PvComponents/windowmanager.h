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
public:
    explicit WindowManager(QObject *parent = 0);

    Q_INVOKABLE void appendWindow(QWindow *window, QString absFilePath, QString macro);
    Q_INVOKABLE void removeWindow(QWindow *window);
    Q_INVOKABLE QWindow* findWindow(QString absFilePath, QString macro);
    Q_INVOKABLE void closeAllWindow();

signals:

public slots:

    void windowDestroyed();

protected:
    QList<WindowEntry> mWindowsList;
};

#endif // WINDOWMANAGER_H
