#ifndef BASEITEM_H
#define BASEITEM_H

#include <QQuickItem>
#include <QCursor>

class BaseItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Qt::CursorShape cursorType READ cursorType WRITE setCursorType NOTIFY cursorTypeChanged)
    Q_PROPERTY(QQuickWindow* baseWindow READ baseWindow)
public:
    explicit BaseItem(QQuickItem *parent = 0);

    Qt::CursorShape cursorType() {return cursor().shape();}
    void setCursorType(Qt::CursorShape cursor) {setCursor(cursor); emit cursorTypeChanged();}

    QQuickWindow * baseWindow() {return this->window();}
signals:
    void cursorTypeChanged();
    void fontChanged();

public slots:

private:
    QFont m_font;
};

#endif // BASEITEM_H
