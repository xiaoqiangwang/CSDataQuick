#include "adimage.h"

ADImage::ADImage(QObject *parent) : QObject(parent)
{

}

void ADImage::setSource(QString source)
{
    _arrayData.setSource(source + 'ArrayData');
    _ndims.setSource(source + 'NDimensions_RBV');
    _dims.setSource(source + 'Dimensions_RBV');
    _color.setSource(source + 'ColorMode_RBV');
}
