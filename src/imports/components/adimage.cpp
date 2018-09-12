/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "adimage.h"

#include <functional>

#include <QtDebug>
#include <QThread>
#include <QtConcurrent>

void imageCleanupHandler(void *data)
{
    delete[] (unsigned char*)data;
}

ADImage::ADImage(QObject *parent) : QObject(parent)
{
    _data = QVariant();
    _lowLevel = 0;
    _highLevel = 255;
    _ratio = 1.0;

    connect(&_arrayData, SIGNAL(valueChanged()), this, SLOT(updateData()));
}

void ADImage::setSource(QString source)
{
    _ndims.setSource(source + "NDimensions_RBV");
    _dims.setSource(source + "Dimensions_RBV");
    _color.setSource(source + "ColorMode_RBV");
    _arrayData.setExtraProperty("UseNativeType", true);
    _arrayData.setSource(source + "ArrayData");
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
    foreach(QVariant v, _dims.value().value<QSequentialIterable>()) {
        int num = v.toInt();
        if (num == 0)
            break;
        dims.append(num);
    }

    _data.setValue(_arrayData.value());
    _dtype = _arrayData.fieldType();

    if (colorMode == 2 && dims.size() == 3) {
    // RGB1
        _format = QImage::Format_RGB888;
        _width = dims[1];
        _height = dims[2];
        _size = _width * _height * 3;
    } else if(colorMode < 2 && dims.size() == 2) {
    // Mono = 0, Bayer = 1
        _format = QImage::Format_Indexed8;
        _width = dims[0];
        _height = dims[1];
        _size = _width * _height;
    } else {
        _format = QImage::Format_Invalid;
    }

    updateImage();
}

template<typename T>
void rescale2(T& v, T* begin, unsigned char *result, double lowLevel, double highLevel, double ratio) {
    result[&v - begin] = (qBound(lowLevel, (double)v, highLevel) - lowLevel) *  ratio;
}

void ADImage::updateImage()
{
    unsigned char *data = new unsigned char[_size];

    switch(_dtype) {
    case QCSData::Char:
    {
        QVector<unsigned char> v8i = _data.value< QVector<unsigned char> >();
        if (v8i.size() < _size) {
            qWarning() << "Inconsistent size " << v8i.size() << " != " << _size;
            return;
        }
        if (_ratio == 1.0)
            memcpy(data, v8i.constData(), _size);
        else
            QtConcurrent::blockingMap(v8i.begin(), v8i.begin() + _size, std::bind(rescale2<unsigned char>, std::placeholders::_1, v8i.data(), data, _lowLevel, _highLevel, _ratio));
    }
        break;
    case QCSData::Short:
    {
        QVector<unsigned short> v16i = _data.value< QVector<unsigned short> >();
        if (v16i.size() < _size)
            return;
        QtConcurrent::blockingMap(v16i.begin(), v16i.begin() + _size, std::bind(rescale2<unsigned short>, std::placeholders::_1, v16i.data(), data, _lowLevel, _highLevel, _ratio));
    }
        break;
    case QCSData::Integer:
    {
        QVector<int> v32i = _data.value< QVector<int> >();
        if (v32i.size() < _size)
            return;
        QtConcurrent::blockingMap(v32i.begin(), v32i.begin() + _size, std::bind(rescale2<int>, std::placeholders::_1, v32i.data(), data, _lowLevel, _highLevel, _ratio));
    }
        break;
    case QCSData::Float:
    {
        QVector<float> v32f = _data.value< QVector<float> >();
        if (v32f.size() < _size)
            return;
        QtConcurrent::blockingMap(v32f.begin(), v32f.begin() + _size, std::bind(rescale2<float>, std::placeholders::_1, v32f.data(), data, _lowLevel, _highLevel, _ratio));
    }
        break;
    case QCSData::Double:
    {
        QVector<double> v64f = _data.value< QVector<double> >();
        if (v64f.size() < _size)
            return;
        QtConcurrent::blockingMap(v64f.begin(), v64f.begin() + _size, std::bind(rescale2<double>, std::placeholders::_1, v64f.data(), data, _lowLevel, _highLevel, _ratio));
    }
        break;
    default:
        qWarning() << "Unsupported data type" << _arrayData.fieldType();
        return;
    }

    emit imageChanged(QImage(data, _width, _height, _format, imageCleanupHandler, data));
}
