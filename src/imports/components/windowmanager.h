#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QList>
#include <QUrl>
#include <QAbstractListModel>

class QWindow;
class QQuickCloseEvent;

#define WindowRole Qt::UserRole + 1
#define PathRole Qt::UserRole + 2
#define MacroRole Qt::UserRole + 3

class WindowListModel : public QAbstractListModel
{
public:
    WindowListModel(QObject *parent=Q_NULLPTR);

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void add(QWindow *window);
    void remove(QWindow *window);
    QWindow* find(QUrl absFilePath, QString macro) const;
    bool contains(QWindow *window) const;

private:
    QList<QWindow*> windows;
};

class WindowManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* entries READ entries CONSTANT)
    Q_PROPERTY(QWindow * mainWindow MEMBER mMainWindow)
public:
    explicit WindowManager(QObject *parent = 0);

    Q_INVOKABLE void appendWindow(QWindow *window);
    Q_INVOKABLE QWindow* findWindow(QUrl absFilePath, QString macro);
    Q_INVOKABLE void closeAllWindow();

    Q_INVOKABLE void closeWindow(QWindow *window);
    Q_INVOKABLE void printWindow(QWindow *window);

    Q_INVOKABLE void setMainWindow(QWindow *window) {mMainWindow = window;}

    void removeWindow(QWindow *window);

    QAbstractListModel *entries();

public slots:
    void windowDestroyed();
    void onClosingWindow(QQuickCloseEvent *event);

protected:
    WindowListModel *model;
    QWindow *mMainWindow;
};

#endif // WINDOWMANAGER_H
