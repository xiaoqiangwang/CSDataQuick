#include "attribute.h"

#include <QtDebug>

DynamicAttributeBase::DynamicAttributeBase(QObject *parent)
    : QObject(parent)
{
    _visibilityCalc = 'A';
}

LimitsBase::LimitsBase(QObject *parent)
    : QObject(parent)
{
    _lopr = 0;
    _loprChannel = 0;
    _loprDefault = 0;
    _loprUser = 0;
    _loprSrc = LimitsSource::Channel;

    _hopr = 1;
    _hoprChannel = 1;
    _hoprDefault = 1;
    _hoprUser = 1;
    _hoprSrc = LimitsSource::Channel;

    _prec = 0;
    _precChannel = 0;
    _precDefault = 0;
    _precUser = 0;
    _precSrc = LimitsSource::Channel;

    connect(this, SIGNAL(loprSrcChanged()), this, SLOT(onLoprSrcChanged()));
    connect(this, SIGNAL(loprChannelChanged()), this, SLOT(onLoprSrcChanged()));
    connect(this, SIGNAL(loprDefaultChanged()), this, SLOT(onLoprSrcChanged()));
    connect(this, SIGNAL(loprUserChanged()), this, SLOT(onLoprSrcChanged()));

    connect(this, SIGNAL(hoprSrcChanged()), this, SLOT(onHoprSrcChanged()));
    connect(this, SIGNAL(hoprChannelChanged()), this, SLOT(onHoprSrcChanged()));
    connect(this, SIGNAL(hoprDefaultChanged()), this, SLOT(onHoprSrcChanged()));
    connect(this, SIGNAL(hoprUserChanged()), this, SLOT(onHoprSrcChanged()));

    connect(this, SIGNAL(precSrcChanged()), this, SLOT(onPrecSrcChanged()));
    connect(this, SIGNAL(precChannelChanged()), this, SLOT(onPrecSrcChanged()));
    connect(this, SIGNAL(precDefaultChanged()), this, SLOT(onPrecSrcChanged()));
    connect(this, SIGNAL(precUserChanged()), this, SLOT(onPrecSrcChanged()));
}

void LimitsBase::onLoprSrcChanged()
{
    double lopr;
    switch (_loprSrc) {
    case LimitsSource::Default:
        lopr = _loprDefault;
        break;
    case LimitsSource::Unused:
    case LimitsSource::Channel:
        lopr = _loprChannel;
        break;
    case LimitsSource::User:
        lopr = _loprUser;
        break;
    }
    if (_lopr != lopr) {
        _lopr = lopr;
        emit loprChanged();
    }
}

void LimitsBase::onHoprSrcChanged()
{
    double hopr;
    switch (_hoprSrc) {
    case LimitsSource::Default:
        hopr = _hoprDefault;
        break;
    case LimitsSource::Unused:
    case LimitsSource::Channel:
        hopr = _hoprChannel;
        break;
    case LimitsSource::User:
        hopr = _hoprUser;
        break;
    }
    if (_hopr != hopr) {
        _hopr = hopr;
        emit hoprChanged();
    }
}

void LimitsBase::onPrecSrcChanged()
{
    int prec;
    switch (_precSrc) {
    case LimitsSource::Default:
        prec = _precDefault;
        break;
    case LimitsSource::Unused:
    case LimitsSource::Channel:
        prec = _precChannel;
        break;
    case LimitsSource::User:
        prec = _precUser;
        break;
    }
    if (_prec != prec) {
        _prec = prec;
        emit precChanged();
    }
}
