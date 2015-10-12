/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "baseitem.h"
#include "attribute.h"

#include <QQuickWindow>

/*!
    \qmltype BaseItem
    \inqmlmodule CSDataQuick.Components
    \brief Base item which all visual items derive.
*/
BaseItem::BaseItem(QQuickItem *parent) :
    QQuickItem(parent)
{
    _dynamicAttribute = new DynamicAttributeBase(this);
    setClip(false);
}

/*!
    \qmlproperty enumeration BaseItem::cursorType
    Mouse cursor shape.
 */
Qt::CursorShape BaseItem::cursorType()
{
    return cursor().shape();
}
void BaseItem::setCursorType(Qt::CursorShape cursor)
{
    setCursor(cursor);
    emit cursorTypeChanged();
}

/*!
    \qmlproperty Window BaseItem::window
    Window which contains this item.
 */
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
    return item->window()->filePath();
}
