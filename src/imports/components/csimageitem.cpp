/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "csimageitem.h"

#include <QPainter>

CSImageItem::CSImageItem(QQuickItem *parent)
    : QQuickPaintedItem( parent )
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons(Qt::LeftButton);
    _colorMap = Gray;
}

void CSImageItem::setImage(QImage image)
{
    if (_image.size() != image.size()) {
        _image = image;
        resetZoom();
    } else
        _image = image;

    if (_image.format() == QImage::Format_Indexed8)
        _image.setColorTable(getColorTable(_colorMap));

    setImplicitSize(_image.width(), _image.height());
    update();
}

void CSImageItem::setColorMap(ColorMapEnum colorMap)
{
    _colorMap = colorMap;
    if (_image.format() == QImage::Format_Indexed8)
        _image.setColorTable(getColorTable(_colorMap));
    update();
}

void CSImageItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_UNUSED(newGeometry);
    Q_UNUSED(oldGeometry);
    resetZoom();
}

void CSImageItem::mousePressEvent(QMouseEvent *event)
{
    _last = event->pos();
}

void CSImageItem::mouseMoveEvent(QMouseEvent *event)
{
    if (_last.isNull())
        return;

    QPointF delta = event->pos() - _last;
    _dest.translate(delta);
    _last = event->pos();
    update();
}

void CSImageItem::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    _last = QPoint();
}

void CSImageItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    resetZoom();
    update();
}

void CSImageItem::wheelEvent(QWheelEvent *event)
{
    QPointF dist = event->posF() - _dest.topLeft();

    if (event->delta() > 0) {
        _dest.setSize(QSizeF(_dest.width() * 0.95, _dest.height() * 0.95));
        _dest.translate(dist * 0.05);
    } else {
        _dest.setSize(QSizeF(_dest.width() * 1.05, _dest.height() * 1.05));
        _dest.translate(-dist * 0.05);
    }
    update();
}

void CSImageItem::paint(QPainter *painter)
{
    if (_image.isNull())
        return;
    painter->drawImage(_dest, _image);
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

void CSImageItem::resetZoom()
{
    if (_image.isNull())
        return;

    _dest = contentsBoundingRect();
    double aspect = _image.width() / _image.height();
    if (_dest.width() / _dest.height() > aspect) {
        double w = _dest.height() * aspect;
        _dest.setX((_dest.width() - w) / 2);
        _dest.setWidth(w);
    } else {
        double h = _dest.width() / aspect;
        _dest.setY((_dest.height() - h) / 2);
        _dest.setHeight(h);
    }
}
