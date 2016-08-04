#include "adimage.h"

#include <QtDebug>
#include <QThread>

void imageCleanupHandler(void *info)
{
    delete[] (unsigned char*)info;
}

ADImage::ADImage(QObject *parent) : QObject(parent)
{
    _lowLevel = 0;
    _highLevel = 255;
    _ratio = 1.0;
}

void ADImage::setSource(QString source)
{
    _arrayData.setSource(source + "ArrayData");
    _ndims.setSource(source + "NDimensions_RBV");
    _dims.setSource(source + "Dimensions_RBV");
    _color.setSource(source + "ColorMode_RBV");

    connect(&_arrayData, SIGNAL(valueChanged()), this, SLOT(updateData()));
}

void ADImage::setLowLevel(double lowLevel)
{
    _lowLevel = lowLevel;
    _ratio = 255. / (_highLevel - _lowLevel);
    updateImage();
}

void ADImage::setHighLevel(double highLevel)
{
    _highLevel = highLevel;
    _ratio = 255. / (_highLevel - _lowLevel);
    updateImage();
}

void ADImage::updateData()
{
    if (!_color.connected() || !_ndims.connected() || !_dims.connected()) {
        return;
    }
    int colorMode = _color.value().toInt();
    if (colorMode > 2) {
        qWarning() << "Unsupported ColorMode" << colorMode;
        return;
    }

    int ndims = _ndims.value().toInt();
    if (ndims != 2 && ndims != 3) {
        qWarning() << "Unsupported rank" << ndims;
        return;
    }

    QVector<int> dims;
    int npixels = 1;
    foreach(QVariant v, _dims.value().value<QSequentialIterable>()) {
        int num = v.toInt();
        if (num == 0)
            break;
        dims.append(num);
        npixels *= num;
    }

    _data.resize(npixels);
    QVector<unsigned char> v = _arrayData.value().value< QVector<unsigned char> >();
    _data = v;
    //for(int i=0; i<v.size()&&i<npixels; i++) {
    //    _data[i] = v.at(i).toDouble();
    //}

    if (colorMode == 2) {
    // RGB1
        _format = QImage::Format_RGB888;
        _width = dims[1];
        _height = dims[2];
    } else {
    // Mono = 0, Bayer = 1
        _format = QImage::Format_Indexed8;
        _width = dims[0];
        _height = dims[1];
    }

    updateImage();
}

void ADImage::updateImage()
{
    QTime t;
    t.start();
    int size = 1;
    if (_format == QImage::Format_Indexed8) {
        size = _width * _height;
    } else {
        size = _width * _height * 3;
    }
    unsigned char *data = new unsigned char[size];
    //memcpy(data, _data.data(), size);
    for (int i=0; i<size; i++) {
        data[i] = (_data[i] - _lowLevel) * _ratio;
    }

    _image = QImage(data, _width, _height, _format, imageCleanupHandler, data);
    emit imageChanged(_image);
}
