/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "shapes.h"

#include <QPainter>
#include <QtMath>

ShapeItem::ShapeItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    _foreground = "black";
    _background = "transparent";
    _lineWidth = 0;
    _fillStyle = FillStyle::Outline;
    _edgeStyle = EdgeStyle::Solid;
}

void ShapeItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
    _path = buildPath();
    update();
}

void ShapeItem::paint(QPainter *painter)
{
    if (_fillStyle == FillStyle::Solid) {
        painter->fillPath(_path, _foreground);
    } else {
        QPen pen;
        if (_edgeStyle == EdgeStyle::Dash)
            pen.setStyle(Qt::DotLine);
        pen.setColor(_foreground);
        pen.setWidth(_lineWidth);
        painter->setPen(pen);
        painter->drawPath(_path);
        if (this->boundingRect() == _path.boundingRect()) {
            setContentsSize(_path.boundingRect().size().toSize() + QSize(1,1));
        }
    }
}

ArcItem::ArcItem(QQuickItem *parent)
    : ShapeItem(parent)
{
    _begin = 0;
    _span = 90;
}

QPainterPath ArcItem::buildPath()
{
    QRectF rc = getDrawArea();
    QPainterPath path;
    if (fillStyle() == FillStyle::Solid)
        path.moveTo(rc.center());
    else
        path.arcMoveTo(rc, _begin);
    path.arcTo(rc, _begin, _span);
    return path;
}

PolylineItem::PolylineItem(QQuickItem *parent)
    : ShapeItem(parent)
{
}

QPainterPath PolylineItem::buildPath()
{
    QPainterPath path;
    path.addPolygon(QPolygonF(_points));
    return path;
}

PolygonItem::PolygonItem(QQuickItem *parent)
    : ShapeItem(parent)
{
}

QPainterPath PolygonItem::buildPath()
{
    QPainterPath path;
    path.addPolygon(QPolygonF(_points));
    path.closeSubpath();
    return path;
}

PaintedRectangletem::PaintedRectangletem(QQuickItem *parent)
    : ShapeItem(parent)
{
}

QPainterPath PaintedRectangletem::buildPath()
{
    QPainterPath path;
    QRectF rc = getDrawArea();
    path.addRect(rc);
    return path;
}

OvalItem::OvalItem(QQuickItem *parent)
    : ShapeItem(parent)
{

}

QPainterPath OvalItem::buildPath()
{
    QPainterPath path;
    path.addEllipse(getDrawArea());
    return path;
}

DoubleRectItem::DoubleRectItem(QQuickItem *parent)
    : ShapeItem(parent)
{

}

QPainterPath DoubleRectItem::buildPath()
{
    QPainterPath path;
    QRectF rect = getDrawArea();

    static QRectF rectangle((float)(4./25.), (float)(4./25.),
                  (float)(13./25.), (float)(14./25.));
    static QVector<QPointF> segmentData = QVector<QPointF>()
        << QPointF((float)(17./25.),(float)(9./25.))
        << QPointF((float)(22./25.),(float)(9./25.))
        << QPointF((float)(22./25.),(float)(22./25.))
        << QPointF((float)(10./25.),(float)(22./25.))
        << QPointF((float)(10./25.),(float)(18./25.));

    QRectF rc(rectangle.x() * rect.width(), rectangle.y() * rect.height(),
                 rectangle.width() * rect.width(), rect.height() * rectangle.height());
    path.addRect(rc);
    QVector<QPointF> points;
    foreach (QPointF point, segmentData) {
       points << QPointF(point.x() * rect.width(), point.y() * rect.height());
    }

    path.addPolygon(QPolygonF(points));
    return path;
}
