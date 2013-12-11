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
    Q_UNUSED(newGeometry);
    Q_UNUSED(oldGeometry);
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
    path.addRect(getDrawArea());
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
