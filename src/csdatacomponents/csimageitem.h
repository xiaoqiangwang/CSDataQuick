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
    explicit CSImageItem(QQuickItem *parent=0);
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

    virtual void paint(QPainter *painter);

protected:
    QVector<QRgb> getColorTable(ColorMapEnum colorMap);

private:
    QImage _image;
    ColorMapEnum _colorMap;
    QMap< ColorMapEnum, QVector<QRgb> > _colorTables;
};

#endif // CSIMAGEITEM_H
