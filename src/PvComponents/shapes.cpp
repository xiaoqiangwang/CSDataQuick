#include "shapes.h"

#include <QPainter>
#include <QtMath>

ShapeItem::ShapeItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    _foreground = "black";
    _background = "transparent";
    _lineWidth = 1;
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
    painter->setRenderHint(QPainter::Qt4CompatiblePainting);
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
    if (_fillStyle == FillStyle::Outline) {
        int offset = lineWidth()%2;
        rc.setRect(offset, offset, rc.width() - offset, rc.height() - offset);
        if (lineWidth() == 1)
            rc.adjust(0, 0, -1, -1);
        else
            rc.adjust(lineWidth()/2, lineWidth()/2, -lineWidth()/2, -lineWidth()/2);
    }
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
    double length = qMin(rect.width(), rect.height()) * 0.4;

    QRectF rc1(3, rect.height() - length - 3, length, length);
    QRectF rc2(3 + length / 2, rect.height() - 3*length/2 - 3, length, length);

    path.addRect(rc1);
    path.addRect(rc2);
    return path;
}
