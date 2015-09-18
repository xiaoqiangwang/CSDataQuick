/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "baseitem.h"

BaseItem::BaseItem(QQuickItem *parent) :
    QQuickItem(parent)
{
    setClip(false);
}
