#include "attribute.h"

DynamicAttributeBase::DynamicAttributeBase(QObject *parent)
    : QObject(parent)
{
    _visibilityCalc = 'A';
}

