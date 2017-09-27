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
    : ShapeItem(parent), _arrowPosition(ArrowPosition::None)
{
    _begin = 0;
    _span = 90;
}

QPainterPath ArcItem::buildPath()
{
    QRectF rc = getDrawArea();
    QPainterPath path;
    if (fillStyle() == FillStyle::Solid) {
        path.moveTo(rc.center());
        path.arcTo(rc, _begin, _span);
    }
    else {
        path.arcMoveTo(rc, _begin);
        QPointF ptStart = path.currentPosition();

        path.arcTo(rc, _begin, _span);
        QPointF ptEnd = path.currentPosition();

        if (_arrowPosition == ArrowPosition::Start || _arrowPosition == ArrowPosition::Both) {
            double angle1 = M_PI_2 -_begin / 180 * M_PI;
            double tx = ptStart.x() - qCos(angle1 - M_PI / 6) * 10;
            double ty = ptStart.y() - qSin(angle1 - M_PI / 6) * 10;
            double bx = ptStart.x() - qCos(angle1 + M_PI / 6) * 10;
            double by = ptStart.y() - qSin(angle1 + M_PI / 6) * 10;
            QPolygonF arrow;
            arrow << QPointF(tx, ty) << ptStart <<  QPointF(bx, by);
            path.addPolygon(arrow);
        }
        if (_arrowPosition == ArrowPosition::End || _arrowPosition == ArrowPosition::Both) {
            double angle1 = M_PI_2 + M_PI - (_begin + _span)/ 180 * M_PI;
            double tx = ptEnd.x() - qCos(angle1 - M_PI / 6) * 10;
            double ty = ptEnd.y() - qSin(angle1 - M_PI / 6) * 10;
            double bx = ptEnd.x() - qCos(angle1 + M_PI / 6) * 10;
            double by = ptEnd.y() - qSin(angle1 + M_PI / 6) * 10;
            QPolygonF arrow;
            arrow << QPointF(tx, ty) << ptEnd <<  QPointF(bx, by);
            path.addPolygon(arrow);
        }
    }
    return path;
}

PolylineItem::PolylineItem(QQuickItem *parent)
    : ShapeItem(parent), _arrowPosition(ArrowPosition::None)
{
}

QPainterPath PolylineItem::buildPath()
{
    QPainterPath path;
    path.addPolygon(QPolygonF(_points));
    if (_points.length() >= 2) {
        if (_arrowPosition == ArrowPosition::Start || _arrowPosition == ArrowPosition::Both) {
            double angle1 = - QLineF(_points.at(1), _points.at(0)).angle() / 180 * M_PI;
            double tx = _points.at(0).x() - qCos(angle1 - M_PI / 6) * 10;
            double ty = _points.at(0).y() - qSin(angle1 - M_PI / 6) * 10;
            double bx = _points.at(0).x() - qCos(angle1 + M_PI / 6) * 10;
            double by = _points.at(0).y() - qSin(angle1 + M_PI / 6) * 10;
            QPolygonF arrow;
            arrow << QPointF(tx, ty) << _points.at(0) <<  QPointF(bx, by);
            path.addPolygon(arrow);
        }
        if (_arrowPosition == ArrowPosition::End || _arrowPosition == ArrowPosition::Both) {
            double angle1 = - QLineF(_points.at(_points.length()-2), _points.last()).angle() / 180 * M_PI;
            double tx = _points.last().x() - qCos(angle1 - M_PI / 6) * 10;
            double ty = _points.last().y() - qSin(angle1 - M_PI / 6) * 10;
            double bx = _points.last().x() - qCos(angle1 + M_PI / 6) * 10;
            double by = _points.last().y() - qSin(angle1 + M_PI / 6) * 10;
            QPolygonF arrow;
            arrow << QPointF(tx, ty) << _points.last() <<  QPointF(bx, by);
            path.addPolygon(arrow);
        }
    }
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
    : ShapeItem(parent), _radiusX(0), _radiusY(0)
{
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
