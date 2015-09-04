#ifndef SHAPES_H
#define SHAPES_H

#include <QQuickPaintedItem>
#include <QVector>
#include <QPoint>
#include <QPainterPath>

#include "enums.h"
#include "utils.h"

class Shape : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor foreground READ foreground WRITE setForeground)
    Q_PROPERTY(EdgeStyle::EdgeStyleEnum edgeStyle READ edgeStyle WRITE setEdgeStyle)
    Q_PROPERTY(FillStyle::FillStyleEnum fillStyle READ fillStyle WRITE setFillStyle)
    Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth)

public:
    Shape(QQuickItem *parent=0);

    QColor foreground() {return _foreground;}
    void setForeground(QColor foreground) {_foreground = foreground; update();}

    EdgeStyle::EdgeStyleEnum edgeStyle() { return _edgeStyle; }
    void setEdgeStyle(EdgeStyle::EdgeStyleEnum style) {_edgeStyle = style;update();}

    FillStyle::FillStyleEnum fillStyle() { return _fillStyle; }
    void setFillStyle(FillStyle::FillStyleEnum style) {_fillStyle = style; rebuildPath(); update();}

    int lineWidth() { return _lineWidth;}
    void setLineWidth(int width) {_lineWidth = width;update();}

    void paint(QPainter * painter);
    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

    void rebuildPath() {_path = buildPath();}

protected:
    /* The subclass must implement this to return a painter path */
    virtual QPainterPath buildPath() = 0;

    QRectF getDrawArea() {
        QRectF boundingRect = contentsBoundingRect();
        int margin = 0; //_lineWidth;
        QRectF rect = QRectF(boundingRect.x() + margin,
                          boundingRect.y() + margin,
                          boundingRect.width() - 2*margin,
                          boundingRect.height() - 2*margin);
        return rect;
    }

private:
    QColor _foreground;
    QColor _background;
    EdgeStyle::EdgeStyleEnum _edgeStyle;
    FillStyle::FillStyleEnum _fillStyle;
    int _lineWidth;
    QPainterPath _path;
};

class Arc : public Shape
{
    Q_OBJECT

    Q_PROPERTY(double begin READ begin WRITE setBegin)
    Q_PROPERTY(double span READ span WRITE setSpan)

public:
    Arc(QQuickItem *parent=0);

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

protected:
    QPainterPath buildPath();

private:
    double _begin, _span;
};

class Polyline : public Shape
{
    Q_OBJECT

    Q_PROPERTY(QVariantList points READ points WRITE setPoints)
public:
    Polyline(QQuickItem *parent=0);

    QVariantList points() {
        QVariantList list;
        foreach(QPoint point, _points)
            list.append(point);
        return list;
    }
    void setPoints(QVariantList points) {
        _points.clear();
        foreach(QVariant v, points)
            _points.append(v.toPoint());
        rebuildPath();
    }

protected:
    QPainterPath buildPath();

private:
    QVector<QPoint> _points;
};

class Polygon : public Shape
{
    Q_OBJECT

    Q_PROPERTY(QVariantList points READ points WRITE setPoints)
public:
    Polygon(QQuickItem *parent=0);

    QVariantList points() {
        QVariantList list;
        foreach(QPoint point, _points)
            list.append(point);
        return list;
    }
    void setPoints(QVariantList points) {
        _points.clear();
        foreach(QVariant v, points)
            _points.append(v.toPoint());
        rebuildPath();
    }

protected:
    QPainterPath buildPath();

private:
    QVector<QPoint> _points;
};

class PaintedRectangle : public Shape
{
    Q_OBJECT
public:
    PaintedRectangle(QQuickItem *parent=0);

protected:
    QPainterPath buildPath();
};

class Oval : public Shape
{
    Q_OBJECT
public:
    Oval(QQuickItem *parent=0);

protected:
    QPainterPath buildPath();
};

class DoubleRect : public Shape
{
    Q_OBJECT
public:
    DoubleRect(QQuickItem *parent=0);

protected:
    QPainterPath buildPath();
};

#endif // SHAPES_H
