#include "csimageitem.h"

#include <QPainter>

CSImageItem::CSImageItem()
{

}

void CSImageItem::paint(QPainter *painter)
{
    painter->drawImage(0, 0, _image);
}
