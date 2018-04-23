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
/*!
    \qmlproperty color BaseItem::foreground
    \brief Color used to draw outline, text.

    This is also the color fill graphics item.
*/
/*!
    \qmlproperty color BaseItem::background
    \brief Color used to fill background.
*/
/*!
    \qmlproperty DynamicAttribute BaseItem::dynamicAttribute
    \brief Configure item's visibility and color
*/
/*!
    \qmlproperty Limits BaseItem::limits
    \brief Configure the operation range and data precision
 */
BaseItem::BaseItem(QQuickItem *parent) :
    QQuickItem(parent)
{
    _foreground = Qt::black;
    _background = Qt::transparent;
    _dynamicAttribute = Q_NULLPTR;
    _limits = Q_NULLPTR;
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

/*!
    \qmlattachedproperty string BaseItem::filePath
    This attached property holds the absolute file path of the display in which this item resides.

    It is attached to each instance of BaseItem. This property maybe give the items awareness of the global context.
*/
QString BaseItemAttached::filePath() const
{
    return item->window()->filePath();
}
