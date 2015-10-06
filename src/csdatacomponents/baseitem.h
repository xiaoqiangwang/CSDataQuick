#ifndef BASEITEM_H
#define BASEITEM_H

#include <QQuickItem>
#include <QCursor>

class BaseItemAttached;

class BaseItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Qt::CursorShape cursorType READ cursorType WRITE setCursorType NOTIFY cursorTypeChanged)
    Q_PROPERTY(QQuickWindow* baseWindow READ baseWindow)
public:
    explicit BaseItem(QQuickItem *parent = 0);

    Qt::CursorShape cursorType();
    void setCursorType(Qt::CursorShape cursor);

    QQuickWindow * baseWindow();

    static BaseItemAttached *qmlAttachedProperties(QObject *);

signals:
    void cursorTypeChanged();
    void fontChanged();

public slots:

private:
    QFont m_font;
};

class BaseItemAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath)
public:
    BaseItemAttached(QObject *object);

    QString filePath() const;

private:
    BaseItem *item;
};


QML_DECLARE_TYPEINFO(BaseItem, QML_HAS_ATTACHED_PROPERTIES)
#endif // BASEITEM_H
