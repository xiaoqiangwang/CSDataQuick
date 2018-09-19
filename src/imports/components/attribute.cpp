/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "attribute.h"
#include "csdata.h"
#include "utils.h"

#include <QtDebug>
/*!
    \qmltype DynamicAttribute
    \inqmlmodule CSDataQuick.Components
    \brief Specifies items visibility.

    Four process variables can be speficied for the calculation.
*/

/*!
    \qmlproperty string DynamicAttribute::channel
    Name of the main process variable
*/
/*!
    \qmlproperty string DynamicAttribute::channelB
    Name of the second process variable
*/
/*!
    \qmlproperty string DynamicAttribute::channelC
    Name of the third process variable
*/
/*!
    \qmlproperty string DynamicAttribute::channelD
    Name of the forth process variable
*/
/*!
    \qmlproperty bool DynamicAttribute::connected
    Whether all process variables are connected
*/
/*!
    \qmlproperty enumeration DynamicAttribute::visibilityMode
    \list
    \li VisibilityMode.Static - No visiblity change.
    \li VisibilityMode.IfZero - Visible if \l channel has value zero.
    \li VisibilityMode.IfNotZero - Visible if \l channel has value no-zero.
    \li VisibilityMode.Calc - Visible if \l visibilityCalc expression returns true.
    \endlist
*/
/*!
    \qmlproperty string DynamicAttribute::visibilityCalc
    An expression that determines whether the object is displayed or not.
    The expression should return 0 for false and anything else for true.
    The following symbols can be used in the expression.
    \list
    \li A - main process variable value
    \li B - second process variable value
    \li C - third process variable value
    \li D - forth process variable value
    \li E - Not used
    \li F - Not used
    \li G - main process variable element count
    \li H - main process variable upper limit
    \li I - main process variable alarm status
    \li J - main process variable alarm severity
    \li K - main process variable precision
    \li L - main process variable lower limit
    \endlist
*/

/*!
    \qmlproperty bool DynamicAttribute::visibility
    true or false as determined by visibilityMode and visibilityCalc.
*/

/*!
    \qmlproperty string DynamicAttribute::altCalc
    An alternative expression with the same syntax as visibilityCalc.
*/

/*!
    \qmlproperty string DynamicAttribute::altCalcResult
    Result from altCalc expression.
*/


DynamicAttribute::DynamicAttribute(QObject *parent)
    : QObject(parent),_pChannel(Q_NULLPTR),_pChannelB(Q_NULLPTR),
      _pChannelC(Q_NULLPTR),_pChannelD(Q_NULLPTR),
      _connected(true),_connectionFlag(0),_connectionMask(0),
      _altCalcResult(0),_visibilityMode(VisibilityMode::Static),_visibility(true)
{
    for (int i=0; i<12; i++)
        _args << 0;
}

DynamicAttribute::~DynamicAttribute()
{
    delete _pChannel;
    delete _pChannelB;
    delete _pChannelC;
    delete _pChannelD;
}

void DynamicAttribute::setChannel(QString channel)
{
    if (channel != _channel) {
        _channel = channel;
        _args[0] = 0;_args[6] = 0;_args[7] = 0;_args[8] = 0;
        _args[9] = 0;_args[10] = 0;_args[11] = 0;
        addChannel(_channel, &_pChannel, 0x01);
        emit channelChanged();
    }
}

void DynamicAttribute::setChannelB(QString channel)
{
    if (channel != _channelB) {
        _channelB = channel;
        _args[1] = 0;
        addChannel(_channelB, &_pChannelB, 0x02);
        emit channelChanged();
    }
}

void DynamicAttribute::setChannelC(QString channel)
{
    if (channel != _channelC) {
        _channelC = channel;
        _args[2] = 0;
        addChannel(_channelC, &_pChannelC, 0x04);
        emit channelChanged();
    }
}

void DynamicAttribute::setChannelD(QString channel)
{
    if (channel != _channelD) {
        _channelD = channel;
        _args[3] = 0;
        addChannel(_channelD, &_pChannelD, 0x08);
        emit channelChanged();
    }
}

void DynamicAttribute::addChannel(QString channel, QCSData **ppChannel, int mask)
{
    if (*ppChannel == Q_NULLPTR)
        *ppChannel = new QCSData();

    if (channel.isEmpty()) {
        disconnect(*ppChannel);
        _connectionFlag &= ~mask;
        _connectionMask &= ~mask;
    } else {
        _connectionFlag &= ~mask;
        _connectionMask |= mask;
        connect(*ppChannel, SIGNAL(valueChanged()), this, SLOT(updateValue()));
        connect(*ppChannel, SIGNAL(connectionChanged()), this, SLOT(updateConnection()));
        // only monitor alarm changes for channel A
        if (mask == 0x01)
            connect(*ppChannel, SIGNAL(alarmChanged()), this, SLOT(updateAlarm()));
    }

    setConnected(_connectionFlag == _connectionMask);
    updateCalc();

    (*ppChannel)->setSource(channel);
}

void DynamicAttribute::setConnected(bool connected)
{
    if (connected == _connected)
        return;
    _connected = connected;
    emit connectionChanged();
}

void DynamicAttribute::setVisibilityMode(VisibilityMode::VisibilityModeEnum mode)
{
    if (mode != _visibilityMode) {
        _visibilityMode = mode;
        emit visibilityModeChanged();
        updateCalc();
    }
}

void DynamicAttribute::setVisibilityCalc(QString calc)
{
    if (calc != _visibilityCalc) {
        _visibilityCalc = calc;
        emit visibilityCalcChanged();

        if (_visibilityMode == VisibilityMode::Calc)
            updateCalc();
    }
}

void DynamicAttribute::setAltCalc(QString calc)
{
    if (calc != _altCalc) {
        _altCalc = calc;
        emit altCalcChanged();
        updateCalc();
    }
}

void DynamicAttribute::updateAlarm()
{
    if (_visibilityMode == VisibilityMode::Calc) {
        _args[8] = _pChannel->alarm()->property("status").toDouble();
        _args[9] = _pChannel->alarm()->property("severity").toDouble();
        updateCalc();
    }
    emit statusChanged();
}

void DynamicAttribute::updateValue()
{
    QCSData *pChannel = qobject_cast<QCSData*>(sender());

    if (pChannel == _pChannel) {
        _args[0] = pChannel->value().toDouble();
        _args[6] = pChannel->count();
        _args[7] = pChannel->range()->property("upper").toDouble();
        _args[10] = pChannel->precision();
        _args[11] = pChannel->range()->property("lower").toDouble();
    } else if (pChannel == _pChannelB) {
        _args[1] = pChannel->value().toDouble();
    } else if (pChannel == _pChannelC) {
        _args[2] = pChannel->value().toDouble();
    } else if (pChannel == _pChannelD) {
        _args[3] = pChannel->value().toDouble();
    }

    updateCalc();
}

void DynamicAttribute::updateConnection()
{
    QCSData *pChannel = qobject_cast<QCSData*>(sender());

    int mask = 0;
    if (pChannel == _pChannel) {
        mask = 0x01;
    } else if (pChannel == _pChannelB) {
        mask = 0x02;
    } else if (pChannel == _pChannelC) {
        mask = 0x04;
    } else if (pChannel == _pChannelD) {
        mask = 0x08;
    } else {
        return;
    }

    if (pChannel->connected())
        _connectionFlag |= mask;
    else
        _connectionFlag &= ~mask;

    setConnected(_connectionFlag == _connectionMask);
}

void DynamicAttribute::updateCalc()
{
    bool visibility = _visibility;

    switch (_visibilityMode) {
    case VisibilityMode::Static:
        visibility = true;
    break;
    case VisibilityMode::IfZero:
        visibility = (_args[0] == 0);
    break;
    case VisibilityMode::IfNotZero:
        visibility = (_args[0] != 0);
    break;
    case VisibilityMode::Calc:
        visibility = (QCSUtils::calculate(_visibilityCalc, _args) != 0);
    break;
    }

    if (visibility != _visibility) {
        _visibility = visibility;
        emit visibilityChanged();
    }

    if (!_altCalc.isEmpty()) {
        double altCalcResult = QCSUtils::calculate(_altCalc, _args);
        if (altCalcResult != _altCalcResult) {
            _altCalcResult = altCalcResult;
            emit altCalcResultChanged();
        }
    }
}

Limits::Limits(QObject *parent)
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

void Limits::onLoprSrcChanged()
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

void Limits::onHoprSrcChanged()
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

void Limits::onPrecSrcChanged()
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
