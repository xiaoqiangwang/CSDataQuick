/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "csdata.h"
#include "csdataenginemanager.h"
#include "csdataengine.h"

#include <QtDebug>

CSDataAlarm::CSDataAlarm(QObject *parent)
    : QObject(parent)
{
    reset();
}

void CSDataAlarm::setAlarm(Severity severity, const QString message)
{
    if (_severity != severity || _message != message) {
        _severity = severity;
        _message = message;
        emit alarmChanged();
    }
}

void CSDataAlarm::reset()
{
    _severity = InvalidAlarm;
    _message = "Undefined";
}


CSDataRange::CSDataRange(QObject *parent)
    : QObject(parent)
{
    reset();
}

void CSDataRange::setRange(double lower, double upper)
{
    if (_lower != lower || _upper != upper) {
        _lower=lower;
        _upper=upper;
        emit rangeChanged();
    }
}


void CSDataRange::reset()
{
    _lower = 0.0;
    _upper = 1.0;
}

CSData::CSData(QObject *parent)
    : QObject(parent),
      _alarm(new CSDataAlarm(this)),
      _range(new CSDataRange(this)),
      _engine(Q_NULLPTR)
{
    reset();
}

CSData::~CSData()
{

}

void CSData::classBegin()
{

}

void CSData::componentComplete()
{
}

QUrl CSData::source() const
{
    return _source;
}

void CSData::setSource(QUrl source)
{
    if (_source == source)
        return;

    // disconnect from current data engine
    if (_engine) {
        _engine->close(this);
        reset();
    }
    _source = source;

    // request data engine
    CSDataEngine * engine = engineManager->engineForName(source.scheme());
    if (!engine) {
        qWarning() << "No CSDataEngine supports" << source;
        return;
    }
    _engine = engine;
    engine->create(this);
    emit sourceChanged();
}

QString CSData::host() const
{
    return _host;
}
void CSData::setHost(const QString host)
{
    if (_host == host)
        return;
    _host = host;
    emit hostChanged();
}

bool CSData::connected() const
{
    return _connected;
}
void CSData::setConnected(bool connected)
{
    if (_connected == connected)
        return;
    _connected = connected;
    emit connectionChanged();
}

CSData::FieldType CSData::fieldType() const
{
    return _fieldType;
}
void CSData::setFieldType(CSData::FieldType fieldType)
{
    if (_fieldType == fieldType)
        return;
    _fieldType = fieldType;
    emit fieldTypeChanged();
}

qulonglong CSData::count() const
{
    return _count;
}
void CSData::setCount(qulonglong count)
{
    if (_count == count)
        return;
    _count = count;
    emit countChanged();
}


CSData::AccessFlags CSData::accessRight() const
{
    return _accessRight;
}
void CSData::setAccessRight(CSData::AccessFlags accessRight)
{
    if (_accessRight == accessRight)
        return;
    _accessRight = accessRight;
    emit accessRightChanged();
}

QDateTime CSData::timeStamp() const
{
    return _timeStamp;
}
void CSData::setTimeStamp(const QDateTime timeStamp)
{
    if (_timeStamp == timeStamp)
        return;
    _timeStamp = timeStamp;
    emit timeStampChanged();
}

int CSData::precision() const
{
    return _precision;
}
void CSData::setPrecision(int precision)
{
    if (_precision == precision)
        return;
    _precision = precision;
    emit precisionChanged();
}

QString CSData::units() const
{
    return _units;
}
void CSData::setUnits(const QString units)
{
    if (_units == units)
        return;
    _units = units;
    emit unitsChanged();
}

QStringList CSData::stateStrings() const
{
    return _stateStrings;
}
void CSData::setStateStrings(const QStringList stateStrings)
{
    if (_stateStrings == stateStrings)
        return;
    _stateStrings = stateStrings;
    emit stateStringsChanged();
}

void CSData::setAlarm(CSDataAlarm::Severity severity, const QString message)
{
    _alarm->setAlarm(severity, message);
}

void CSData::setRange(double lower, double upper)
{
    _range->setRange(lower, upper);
}

QVariant CSData::value() const
{
    return _value;
}

void CSData::setValue(const QVariant value)
{
    if (_value == value)
        return;

    _value.setValue(value);
    emit valueChanged();
}

void CSData::updateValue(const QVariant value)
{
    if (_engine)
        _engine->setValue(this, value);
}

void CSData::reset()
{
    _source.clear();
    _host.clear();
    _connected = false;

    _fieldType = Invalid;
    _count = 0;
    _value = 0.0;

    _accessRight = NoAccess;
    _alarm->reset();
    _timeStamp.setMSecsSinceEpoch(0);

    _precision = 0;
    _units.clear();
    _range->reset();

    _stateStrings.clear();
}

