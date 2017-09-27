#ifndef SHAPES_H
#define SHAPES_H

#include <QQuickPaintedItem>
#include <QVector>
#include <QPointF>
#include <QPainterPath>

#include "enums.h"
#include "utils.h"

class ShapeItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor foreground READ foreground WRITE setForeground)
    Q_PROPERTY(EdgeStyle::EdgeStyleEnum edgeStyle READ edgeStyle WRITE setEdgeStyle)
    Q_PROPERTY(FillStyle::FillStyleEnum fillStyle READ fillStyle WRITE setFillStyle)
    Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth)

public:
    ShapeItem(QQuickItem *parent=0);

    QColor foreground() {return _foreground;}
    void setForeground(QColor foreground) {_foreground = foreground; update();}

    EdgeStyle::EdgeStyleEnum edgeStyle() { return _edgeStyle; }
    void setEdgeStyle(EdgeStyle::EdgeStyleEnum style) {_edgeStyle = style;update();}

    FillStyle::FillStyleEnum fillStyle() { return _fillStyle; }
    void setFillStyle(FillStyle::FillStyleEnum style) {_fillStyle = style; rebuildPath(); update();}

    int lineWidth() { return _lineWidth;}
    void setLineWidth(int width) {_lineWidth = width; update();}

    void paint(QPainter * painter);
    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

    void rebuildPath() {_path = buildPath();}

protected:
    /* The subclass must implement this to return a painter path */
    virtual QPainterPath buildPath() = 0;

    QRectF getDrawArea() {
        QRectF boundingRect = contentsBoundingRect();
        int margin = _fillStyle == FillStyle::Outline ?  (_lineWidth + 1) /2 : 0;
        QRectF rect = QRectF(boundingRect.x() + margin,
                          boundingRect.y() + margin,
                          boundingRect.width() - 2*margin,
                          boundingRect.height() - 2*margin);
        return rect;
    }

protected:
    QColor _foreground;
    QColor _background;
    EdgeStyle::EdgeStyleEnum _edgeStyle;
    FillStyle::FillStyleEnum _fillStyle;
    int _lineWidth;
    QPainterPath _path;
};

class ArcItem : public ShapeItem
{
    Q_OBJECT

    Q_PROPERTY(double begin READ begin WRITE setBegin)
    Q_PROPERTY(double span READ span WRITE setSpan)
    Q_PROPERTY(ArrowPosition::ArrowPositionEnum arrowPosition MEMBER _arrowPosition WRITE setArrowPosition)

public:
    ArcItem(QQuickItem *parent=0);

    double begin() { return _begin; }
    void setBegin(double begin) {
        _begin = begin;
        rebuildPath();
        update();
    }

    double span() { return _span; }
    void setSpan(double span) {
        _span = span;
        rebuildPath();
        update();
    }

    void setArrowPosition(ArrowPosition::ArrowPositionEnum arrowPosition) {
        _arrowPosition = arrowPosition;
        rebuildPath();
        update();
    }

protected:
    QPainterPath buildPath();

private:
    double _begin, _span;
    ArrowPosition::ArrowPositionEnum _arrowPosition;
};

class PolylineItem : public ShapeItem
{
    Q_OBJECT

    Q_PROPERTY(ArrowPosition::ArrowPositionEnum arrowPosition MEMBER _arrowPosition WRITE setArrowPosition)
    Q_PROPERTY(QVariantList points READ points WRITE setPoints NOTIFY pointsChanged)
public:
    PolylineItem(QQuickItem *parent=0);

    QVariantList points() {
        QVariantList list;
        foreach(QPointF point, _points)
            list.append(point);
        return list;
    }
    void setPoints(QVariantList points) {
        _points.clear();
        foreach(QVariant v, points)
            _points.append(v.toPointF());
        emit pointsChanged();
        rebuildPath();
        update();
    }
    void setArrowPosition(ArrowPosition::ArrowPositionEnum arrowPosition) {
        _arrowPosition = arrowPosition;
        rebuildPath();
        update();
    }
signals:
    void pointsChanged();

protected:
    QPainterPath buildPath();

private:
    ArrowPosition::ArrowPositionEnum _arrowPosition;
    QVector<QPointF> _points;
};

class PolygonItem : public ShapeItem
{
    Q_OBJECT

    Q_PROPERTY(QVariantList points READ points WRITE setPoints NOTIFY pointsChanged)
public:
    PolygonItem(QQuickItem *parent=0);

    QVariantList points() {
        QVariantList list;
        foreach(QPointF point, _points)
            list.append(point);
        return list;
    }
    void setPoints(QVariantList points) {
        _points.clear();
        foreach(QVariant v, points)
            _points.append(v.toPointF());
        emit pointsChanged();
        rebuildPath();
        update();
    }

signals:
    void pointsChanged();

protected:
    QPainterPath buildPath();

private:
    QVector<QPointF> _points;
};

class PaintedRectangletem : public ShapeItem
{
    Q_OBJECT
    Q_PROPERTY(double radiusX MEMBER _radiusX WRITE setRadiusX)
    Q_PROPERTY(double radiusY MEMBER _radiusY WRITE setRadiusY)
public:
    PaintedRectangletem(QQuickItem *parent=0);

    void setRadiusX(double rx) {
        _radiusX = rx;
        rebuildPath();
        update();
    }
    void setRadiusY(double ry) {
        _radiusY = ry;
        rebuildPath();
        update();
    }
protected:
    QPainterPath buildPath();
private:
    double _radiusX;
    double _radiusY;
};

class OvalItem : public ShapeItem
{
    Q_OBJECT
public:
    OvalItem(QQuickItem *parent=0);

protected:
    QPainterPath buildPath();
};

class DoubleRectItem : public ShapeItem
{
    Q_OBJECT
public:
    DoubleRectItem(QQuickItem *parent=0);

protected:
    QPainterPath buildPath();
};

#endif // SHAPES_H
