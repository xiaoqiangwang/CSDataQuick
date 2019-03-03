#ifndef CSIMAGEITEM_H
#define CSIMAGEITEM_H

#include <QQuickPaintedItem>
#include <QImage>

class CSImageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_ENUMS(ColorMapEnum)
    Q_PROPERTY(ColorMapEnum colorMap MEMBER _colorMap WRITE setColorMap)

public:
    explicit CSImageItem(QQuickItem *parent=Q_NULLPTR);
    enum ColorMapEnum {
        Gray = 0,
        Hot,
        Cool,
        Spring,
        Summer,
        Autumn,
        Winter
    };

    Q_INVOKABLE void setImage(QImage image);

    void setColorMap(ColorMapEnum colorMap);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    virtual void paint(QPainter *painter);

protected:
    QVector<QRgb> getColorTable(ColorMapEnum colorMap);
    void resetZoom();

private:
    QImage _image;
    QRectF _dest;
    QPointF _last;
    ColorMapEnum _colorMap;
    QMap< ColorMapEnum, QVector<QRgb> > _colorTables;
};

#endif // CSIMAGEITEM_H
