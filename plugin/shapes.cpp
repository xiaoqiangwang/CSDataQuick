#include "shapes.h"

#include <QPainter>

Shape::Shape(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    _foreground = "black";
    _background = "transparent";
    _lineWidth = 1;
    _fillStyle = FillStyle::Outline;
    _edgeStyle = EdgeStyle::Solid;
}

void Shape::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
    _path = buildPath();
    update();
}

void Shape::paint(QPainter *painter)
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
    }
}

Arc::Arc(QQuickItem *parent)
    : Shape(parent)
{
    _begin = 0;
    _span = 90;
}

QPainterPath Arc::buildPath()
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

Polyline::Polyline(QQuickItem *parent)
    : Shape(parent)
{
}

QPainterPath Polyline::buildPath()
{
    QPainterPath path;
    path.addPolygon(QPolygonF(_points));
    return path;
}

Polygon::Polygon(QQuickItem *parent)
    : Shape(parent)
{
}

QPainterPath Polygon::buildPath()
{
    QPainterPath path;
    path.addPolygon(QPolygonF(_points));
    path.closeSubpath();
    return path;
}

PaintedRectangle::PaintedRectangle(QQuickItem *parent)
    : Shape(parent)
{
}

QPainterPath PaintedRectangle::buildPath()
{
    QPainterPath path;
    QRectF rc = getDrawArea();
    rc.setWidth(rc.width() - lineWidth());
    rc.setHeight(rc.height() - lineWidth());
    path.addRect(rc);
    return path;
}

Oval::Oval(QQuickItem *parent)
    : Shape(parent)
{

}

QPainterPath Oval::buildPath()
{
    QPainterPath path;
    path.addEllipse(getDrawArea());
    return path;
}

DoubleRect::DoubleRect(QQuickItem *parent)
    : Shape(parent)
{

}

QPainterPath DoubleRect::buildPath()
{
    QPainterPath path;
    QRectF rect = getDrawArea();
    double length = qMin(rect.width(), rect.height()) * 0.4;

    QRectF rc1(3, rect.height() - length - 3, length, length);
    QRectF rc2(3 + length / 2, rect.height() - 3*length/2 - 3, length, length);

    path.addRect(rc1);
    path.addRect(rc2);
    return path;
}
