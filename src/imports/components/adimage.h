#ifndef ADIMAGE_H
#define ADIMAGE_H

#include <QObject>
#include <QImage>

#include "csdata.h"

class ADImage : public QObject
{
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(double lowLevel  MEMBER _lowLevel  WRITE setLowLevel NOTIFY lowLevelChanged)
    Q_PROPERTY(double highLevel MEMBER _highLevel WRITE setHighLevel NOTIFY highLevelChanged)

    Q_OBJECT
public:
    explicit ADImage(QObject *parent = Q_NULLPTR);

    QString source() { return _source; }
    void setSource(QString source);

    void setLowLevel(double lowLevel);
    void setHighLevel(double highLevel);

signals:
    void sourceChanged();
    void lowLevelChanged();
    void highLevelChanged();
    void imageChanged(QImage image);

public slots:
    void updateData();

protected:
    void updateImage();

private:
    QString _source;
    double _lowLevel;
    double _highLevel;
    double _ratio;

    QVariant _data;
    QCSData::FieldType _dtype;
    int _width;
    int _height;
    int _size;
    QImage::Format _format;

    QCSData _arrayData;
    QCSData _ndims;
    QCSData _dims;
    QCSData _color;
};

#endif // ADIMAGE_H
