#ifndef BASEITEM_H
#define BASEITEM_H

#include <QQuickItem>
#include <QCursor>

class BaseItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Qt::CursorShape cursorType READ cursorType WRITE setCursorType NOTIFY cursorTypeChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QQuickWindow* baseWindow READ baseWindow)
public:
    explicit BaseItem(QQuickItem *parent = 0);

    Qt::CursorShape cursorType() {return cursor().shape();}
    void setCursorType(Qt::CursorShape cursor) {setCursor(cursor); emit cursorTypeChanged();}

    QFont font() {return m_font;}
    void setFont(const QFont font) {m_font = font; emit fontChanged();}

    QQuickWindow * baseWindow() {return this->window();}
signals:
    void cursorTypeChanged();
    void fontChanged();

public slots:

private:
    QFont m_font;
};

#endif // BASEITEM_H
