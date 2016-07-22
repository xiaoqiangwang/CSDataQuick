#ifndef CSIMAGEITEM_H
#define CSIMAGEITEM_H

#include <QQuickPaintedItem>
#include <QImage>

class CSImageItem : public QQuickPaintedItem
{
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)

public:
    CSImageItem();

    QImage image() { return _image; }
    void setImage(QImage image) { _image = image; update(); emit imageChanged(); }

    virtual void paint(QPainter *painter);

signals:
    void imageChanged();

private:
    QImage _image;
};

#endif // CSIMAGEITEM_H
