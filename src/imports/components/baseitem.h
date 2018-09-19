#ifndef BASEITEM_H
#define BASEITEM_H

#include <QQuickItem>
#include <QCursor>

class BaseItemAttached;
class DynamicAttribute;
class Limits;

class BaseItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor foreground MEMBER _foreground NOTIFY foregroundChanged)
    Q_PROPERTY(QColor background MEMBER _background NOTIFY backgroundChanged)
    Q_PROPERTY(Qt::CursorShape cursorType READ cursorType WRITE setCursorType NOTIFY cursorTypeChanged)
    Q_PROPERTY(QQuickWindow* baseWindow READ baseWindow)
    Q_PROPERTY(DynamicAttribute* dynamicAttribute READ dynamicAttribute CONSTANT)
    Q_PROPERTY(Limits* limits READ limits CONSTANT)

public:
    explicit BaseItem(QQuickItem *parent = 0);

    Qt::CursorShape cursorType();
    void setCursorType(Qt::CursorShape cursor);

    QQuickWindow * baseWindow();
    DynamicAttribute * dynamicAttribute();
    Limits * limits();

    static BaseItemAttached *qmlAttachedProperties(QObject *);

signals:
    void backgroundChanged();
    void cursorTypeChanged();
    void foregroundChanged();

public slots:

private:
    QColor _foreground;
    QColor _background;
    DynamicAttribute *_dynamicAttribute;
    Limits *_limits;
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


QML_DECLARE_TYPE(BaseItem)
QML_DECLARE_TYPEINFO(BaseItem, QML_HAS_ATTACHED_PROPERTIES)
#endif // BASEITEM_H
