#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QQuickWindow>
#include <QFileInfo>

class BaseWindow : public QQuickWindow
{
    Q_OBJECT

    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString macro MEMBER macro)

public:
    explicit BaseWindow(QWindow *parent=0);
    virtual ~BaseWindow();

    Q_INVOKABLE void printWindow();

    QString filePath() const;
    void setFilePath(const QString &filePath);

signals:
    void filePathChanged();

private:
    QString macro;

    Q_DISABLE_COPY(BaseWindow)
};

#endif
