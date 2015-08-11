#ifndef BASEITEM_H
#define BASEITEM_H

#include <QQuickItem>
#include <QCursor>

class BaseItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Qt::CursorShape cursorType READ cursorType WRITE setCursorType NOTIFY cursorTypeChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
public:
    explicit BaseItem(QQuickItem *parent = 0);

    Qt::CursorShape cursorType() {return cursor().shape();}
    void setCursorType(Qt::CursorShape cursor) {setCursor(cursor); qDebug() << cursor; emit cursorTypeChanged();}

    QFont font() {}
    void setFont(const QFont font) {}

signals:
    void cursorTypeChanged();
    void fontChanged();

public slots:

private:
};

#endif // BASEITEM_H
