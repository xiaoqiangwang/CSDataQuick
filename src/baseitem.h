#ifndef BASEITEM_H
#define BASEITEM_H

#include <QQuickItem>
#include <QCursor>

class BaseItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Qt::CursorShape cursorType READ cursorType WRITE setCursorType NOTIFY cursorTypeChanged)

public:
    explicit BaseItem(QQuickItem *parent = 0);

    Qt::CursorShape cursorType() {return cursor().shape();}
    void setCursorType(Qt::CursorShape cursor) {setCursor(cursor); qDebug() << cursor; emit cursorTypeChanged();}

signals:
    void cursorTypeChanged();

public slots:

private:
};

#endif // BASEITEM_H
