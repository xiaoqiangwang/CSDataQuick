#include "baseitem.h"

BaseItem::BaseItem(QQuickItem *parent) :
    QQuickItem(parent)
{
    setClip(false);
}
