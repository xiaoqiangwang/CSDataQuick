/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
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

QRectF ShapeItem::getDrawArea()
{
    QRectF boundingRect = contentsBoundingRect();
    int margin = _fillStyle == FillStyle::Outline ?  qMax(1, (_lineWidth + 1) /2) : 0;
    QRectF rect = QRectF(boundingRect.x() + margin,
                      boundingRect.y() + margin,
                      boundingRect.width() - 2*margin,
                      boundingRect.height() - 2*margin);
    return rect;
}

QPolygonF ShapeItem::createArrow(const QPointF pt, double angle)
{
    double tx = pt.x() - qCos(angle - M_PI / 6) * 10;
    double ty = pt.y() - qSin(angle - M_PI / 6) * 10;
    double bx = pt.x() - qCos(angle + M_PI / 6) * 10;
    double by = pt.y() - qSin(angle + M_PI / 6) * 10;
    QPolygonF arrow;
    arrow << QPointF(tx, ty) << pt <<  QPointF(bx, by);
    return arrow;
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
    : ShapeItem(parent), _closure(ClosureEnum::Open), _arrowPosition(ArrowPosition::None)
{
    _begin = 0;
    _span = 90;
}

void ArcItem::setBegin(double begin)
{
    _begin = begin;
    rebuildPath();
    update();
}

void ArcItem::setSpan(double span)
{
    _span = span;
    rebuildPath();
    update();
}

void ArcItem::setClosure(ClosureEnum closure)
{
    _closure = closure;
    rebuildPath();
    update();
}

void ArcItem::setArrowPosition(ArrowPosition::ArrowPositionEnum arrowPosition)
{
    _arrowPosition = arrowPosition;
    rebuildPath();
    update();
}

QPainterPath ArcItem::buildPath()
{
    QRectF rc = getDrawArea();
    QPainterPath path;
    if (fillStyle() == FillStyle::Solid) {
        if (_closure == ClosureEnum::Chord)
            path.arcMoveTo(rc, _begin);
        else
            path.moveTo(rc.center());
        path.arcTo(rc, _begin, _span);
    }
    else {
        if (_closure == ClosureEnum::Pie) 
            path.moveTo(rc.center());
        else
            path.arcMoveTo(rc, _begin);
        QPointF ptStart = path.currentPosition();

        path.arcTo(rc, _begin, _span);
        QPointF ptEnd = path.currentPosition();

        if (_closure != ClosureEnum::Open)
            path.closeSubpath();

        if (_arrowPosition == ArrowPosition::Start || _arrowPosition == ArrowPosition::Both) {
            double angle = M_PI_2 -_begin / 180 * M_PI;
            path.addPolygon(createArrow(ptStart, angle));
        }
        if (_arrowPosition == ArrowPosition::End || _arrowPosition == ArrowPosition::Both) {
            double angle = M_PI_2 + M_PI - (_begin + _span)/ 180 * M_PI;
            path.addPolygon(createArrow(ptEnd, angle));
        }
    }
    return path;
}

PolylineItem::PolylineItem(QQuickItem *parent)
    : ShapeItem(parent), _arrowPosition(ArrowPosition::None)
{
}

QVariantList PolylineItem::points()
{
    QVariantList list;
    foreach(QPointF point, _points)
        list.append(point);
    return list;
}

void PolylineItem::setPoints(QVariantList points)
{
    _points.clear();
    foreach(QVariant v, points)
        _points.append(v.toPointF());
    emit pointsChanged();
    rebuildPath();
    update();
}

void PolylineItem::setArrowPosition(ArrowPosition::ArrowPositionEnum arrowPosition)
{
    _arrowPosition = arrowPosition;
    rebuildPath();
    update();
}

QPainterPath PolylineItem::buildPath()
{
    QPainterPath path;
    path.addPolygon(QPolygonF(_points));
    if (_points.length() >= 2) {
        if (_arrowPosition == ArrowPosition::Start || _arrowPosition == ArrowPosition::Both) {
            double angle = - QLineF(_points.at(1), _points.at(0)).angle() / 180 * M_PI;
            path.addPolygon(createArrow(_points.first(), angle));
        }
        if (_arrowPosition == ArrowPosition::End || _arrowPosition == ArrowPosition::Both) {
            double angle = - QLineF(_points.at(_points.length()-2), _points.last()).angle() / 180 * M_PI;
            path.addPolygon(createArrow(_points.last(), angle));
        }
    }
    setImplicitWidth(boundingRect().width());
    setImplicitHeight(boundingRect().height());
    return path;
}

PolygonItem::PolygonItem(QQuickItem *parent)
    : ShapeItem(parent)
{
}

QVariantList PolygonItem::points()
{
    QVariantList list;
    foreach(QPointF point, _points)
        list.append(point);
    return list;
}

void PolygonItem::setPoints(QVariantList points)
{
    _points.clear();
    foreach(QVariant v, points)
        _points.append(v.toPointF());
    emit pointsChanged();
    rebuildPath();
    update();
}

QPainterPath PolygonItem::buildPath()
{
    QPainterPath path;
    path.addPolygon(QPolygonF(_points));
    path.closeSubpath();

    setImplicitWidth(boundingRect().width());
    setImplicitHeight(boundingRect().height());
    return path;
}

PaintedRectangletem::PaintedRectangletem(QQuickItem *parent)
    : ShapeItem(parent), _radiusX(0), _radiusY(0)
{
}

void PaintedRectangletem::setRadiusX(double rx)
{
    _radiusX = rx;
    rebuildPath();
    update();
}

void PaintedRectangletem::setRadiusY(double ry)
{
    _radiusY = ry;
    rebuildPath();
    update();
}

QPainterPath PaintedRectangletem::buildPath()
{
    QPainterPath path;
    QRectF rc = getDrawArea();
    path.addRoundedRect(rc, _radiusX, _radiusY);
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
