/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "baseitem.h"

#include <QQuickWindow>

BaseItem::BaseItem(QQuickItem *parent) :
    QQuickItem(parent)
{
    setClip(false);
}

Qt::CursorShape BaseItem::cursorType()
{
    return cursor().shape();
}
void BaseItem::setCursorType(Qt::CursorShape cursor)
{
    setCursor(cursor);
    emit cursorTypeChanged();
}

QQuickWindow * BaseItem::baseWindow()
{
    return this->window();
}

BaseItemAttached *BaseItem::qmlAttachedProperties(QObject *object)
{
    return new BaseItemAttached(object);
}

BaseItemAttached::BaseItemAttached(QObject *object)
{
    item = qobject_cast<BaseItem*>(object);
}

QString BaseItemAttached::filePath() const
{
    qDebug() << item->window()->title();
    return item->window()->filePath();
}
