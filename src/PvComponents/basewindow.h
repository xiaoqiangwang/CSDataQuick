#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QQuickWindow>

class BaseWindow : public QQuickWindow
{
    Q_OBJECT

    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString macro READ macro WRITE setMacro NOTIFY macroChanged)

public:
    explicit BaseWindow(QWindow *parent=0);
    virtual ~BaseWindow();

    Q_INVOKABLE void printWindow();

    QString filePath() const;
    void setFilePath(const QString &filePath);

    QString macro() const;
    void setMacro(const QString &macro);

signals:
    void filePathChanged();
    void macroChanged();

private:
    QString _macro;

    Q_DISABLE_COPY(BaseWindow)
};

#endif
