#include "csimageitem.h"

#include <QPainter>

CSImageItem::CSImageItem(QQuickItem *parent)
    : QQuickPaintedItem( parent )
{
    setFlag( QQuickItem::ItemHasContents, true );
    _colorMap = Gray;
}

void CSImageItem::setImage(QImage image)
{
    _image = image;
    _image.setColorTable(getColorTable(_colorMap));
    update();
}

void CSImageItem::setColorMap(ColorMapEnum colorMap)
{
    _colorMap = colorMap;
    _image.setColorTable(getColorTable(_colorMap));
    update();
}

void CSImageItem::paint(QPainter *painter)
{
    if (_image.isNull())
        return;
    QSize sz = _image.size();
    painter->drawImage(contentsBoundingRect(), _image);
}

QVector<QRgb> CSImageItem::getColorTable(ColorMapEnum colorMap)
{
    if (_colorTables.contains(colorMap))
        return _colorTables[colorMap];

    QVector<QRgb> ct(256);
    switch (colorMap) {
    case Gray:
        for(int i=0; i<256; i++) {
            ct[i] = qRgb(i, i, i);
        }
        break;
    case Hot:
        for(int i=0; i<96; i++) {
            ct[i] = qRgb((i+1) * 255/96, 0, 0);
        }
        for(int i=96; i<192; i++) {
            ct[i] = qRgb(255, (i-95) * 255/96, 0);
        }
        for (int i=192; i<256; i++) {
            ct[i] = qRgb(255, 255, (i-191)*255/64);
        }
        break;
    case Cool:
        for(int i=0; i<256; i++) {
            ct[i] = qRgb(i, 255-i, 255);
        }
        break;
    case Spring:
        for(int i=0; i<256; i++) {
            ct[i] = qRgb(255, i, 255-i);
        }
        break;
    case Summer:
        for(int i=0; i<256; i++) {
            ct[i] = qRgb(i, 127+(i+1)/2, 128);
        }
        break;
    case Autumn:
        for(int i=0; i<256; i++) {
            ct[i] = qRgb(255, i, 0);
        }
        break;
    case Winter:
        for(int i=0; i<256; i++) {
            ct[i] = qRgb(0, 255-(i+1)/2, 0);
        }
        break;
    }
    _colorTables[colorMap] = ct;
    return ct;
}
