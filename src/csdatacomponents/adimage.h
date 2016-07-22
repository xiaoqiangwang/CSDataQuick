#ifndef ADIMAGE_H
#define ADIMAGE_H

#include <QObject>
#include <QImage>

#include "csdata.h"

class ADImage : public QObject
{
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(double highLevel READ highLevel WRITE setHighLevel NOTIFY highLevelChanged)
    Q_PROPERTY(double lowLevel READ lowLevel WRITE setLowLevel NOTIFY lowLevelChanged)

    Q_OBJECT
public:
    explicit ADImage(QObject *parent = 0);

    QString source() { return _source; }
    void setSource(QString source);

    QImage image() { return _image; }
    void setImage(QImage image) { _image = image; emit imageChanged(); }

signals:
    void sourceChanged();
    void imageChanged();

public slots:

private:
    QString _source;
    QImage _image;

    QCSData _arrayData;
    QCSData _ndims;
    QCSData _dims;
    QCSData _color;
};

#endif // ADIMAGE_H
