/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "csdataenginemanager.h"
#include "csdataengine.h"

#include "csdata.h"

#include <QCoreApplication>
#include <QtDebug>
/*!
    \class QCSDataAlarm
    \inmodule CSDataQuick.Data
    \brief The CSDataAlarm class provides alarm information about the data.
*/
/*!
    \qmltype CSDataAlarm
    \inqmlmodule CSDataQuick.Data
    \instantiates QCSDataAlarm
    \brief The object provides alarm information about the data.
*/
/*!
    \enum QCSDataAlarm::Severity
        \value NoAlarm
               Normal state
        \value MinorAlarm
               Lowest alarm severity
        \value MajorAlarm
               Highest alarm severity
        \value InvalidAlarm
               Invalid data or no communication.
*/
/*!
    \property QCSDataAlarm::severity
    This property holds the alarm severity.

    \sa QCSDataAlarm::Severity
*/
/*!
    \qmlproperty enumeration CSDataAlarm::severity
    This property holds the alarm severity.

    \sa QCSDataAlarm::Severity
*/
/*!
    \property QCSDataAlarm::status
    This property holds the alarm status.

    The actual meaning depends on data engine.
*/
/*!
    \qmlproperty int CSDataAlarm::status
    This property holds the alarm status.
*/
/*!
    \property QCSDataAlarm::message
    This property holds the alarm message.
*/
/*!
    \qmlproperty string CSDataAlarm::message
    This property holds the alarm message.
*/
/*!
    \brief Construct alarm information object with the given \a parent.
 */
QCSDataAlarm::QCSDataAlarm(QObject *parent)
    : QObject(parent)
{
    reset();

}
/*!
    \brief Set the alarm \a severity, \a status and \a message.
 */
void QCSDataAlarm::setAlarm(Severity severity, int status, const QString message)
{
    if (_severity != severity || _status != status || _message != message) {
        _severity = severity;
        _status = status;
        _message = message;
        emit alarmChanged();
    }
}
/*! \internal */
void QCSDataAlarm::reset()
{
    _severity = InvalidAlarm;
    _status = 0;
    _message = "Undefined";
}

/*!
    \class QCSDataRange
    \inmodule CSDataQuick.Data
    \brief This class provides limits information about the data.
*/
/*!
    \qmltype CSDataRange
    \inqmlmodule CSDataQuick.Data
    \instantiates QCSDataRange
    \brief This class provides limits information about the data.
*/
/*!
    \property QCSDataRange::lower
    This property holds the lower limit.
*/
/*!
    \qmlproperty real CSDataRange::lower
    This property holds the lower limit.
*/
/*!
    \property QCSDataRange::upper
    This property holds the upper limit.
*/
/*!
    \qmlproperty real CSDataRange::upper
    This property holds the upper limit.
*/
/*!
    \brief Construct a range object with the given \a parent.
 */
QCSDataRange::QCSDataRange(QObject *parent)
    : QObject(parent)
{
    reset();
}
/*!
    \brief Set the \a lower and \a upper limits .
 */
void QCSDataRange::setRange(double lower, double upper)
{
    if (_lower != lower || _upper != upper) {
        _lower=lower;
        _upper=upper;
        emit rangeChanged();
    }
}
/*!
    \brief Return true if lower is less than upper.
 */
bool QCSDataRange::isValid() const
{
    return _lower < _upper;
}
/*! \internal */
void QCSDataRange::reset()
{
    _lower = 0.0;
    _upper = 1.0;
}

/*!
    \class QCSData
    \inmodule CSDataQuick.Data
    \brief This class provides access to control system data.
*/
/*!
    \qmltype CSData
    \inqmlmodule CSDataQuick.Data
    \instantiates QCSData
    \brief Object that provides access to control system data.

    Example:
    \qml
        CSData {
            id: voltagePV
            source: 'MTEST:Voltage'
        }
    \endqml
*/
/*!
    \enum QCSData::FieldType
        \value String
               Fixed length string of 40 charactors.
        \value Integer
               16 bit integer number
        \value Short
               Same as QCSData::Integer
        \value Float
               32 bit float number
        \value Enum
               An enumeration of choices
        \value Char
               8 bit char
        \value Long
               32 bit integer number
        \value Double
               64 but float number
        \value Invalid
               Invalid or disconnected PV object
*/
/*!
    \enum QCSData::AccessFlag
        \value NoAccess
               No access to the data.
        \value ReadAccess
               Right to read.
        \value WriteAccess
               Right to write.
*/
/*!
    \brief Construct a data object with the given \a parent.
 */
QCSData::QCSData(QObject *parent)
    : QObject(parent),
      _alarm(new QCSDataAlarm(this)),
      _range(new QCSDataRange(this)),
      _engine(Q_NULLPTR)
{
    reset();
    connect(_alarm, SIGNAL(alarmChanged()), this, SIGNAL(alarmChanged()));
    connect(_range, SIGNAL(rangeChanged()), this, SIGNAL(rangeChanged()));

    _inPuppet = false;

    if (qgetenv("QML_PUPPET_MODE") == "true") {
        _inPuppet = true;
        _connected = true;
        _fieldType = Double;
        _count = 1;
        _accessRight = ReadAccess | WriteAccess;
        _stateStrings << "OFF" << "ON";
        _alarm->setAlarm(QCSDataAlarm::NoAlarm, 0, "NoAlarm");
    }
    _extraProperties["QmlPuppetMode"] = _inPuppet;
}
/*!
    \brief Delete the data object and disconnect from data engine.
 */
QCSData::~QCSData()
{
    // disconnect from current data engine
    if (_engine) {
        _engine->close(this);
        reset();
    }
}

/*!
    \property QCSData::source
    This property holds the data's source name.
*/
/*!
    \qmlproperty string CSData::source
    This property holds the data's source name.
*/
QString QCSData::source() const
{
    return _source;
}
void QCSData::setSource(const QString source)
{
    if (_source == source)
        return;

    if (_inPuppet) {
        _source = source;
        emit sourceChanged();
        return;
    }
    // request data engine
    QCSDataEngine * engine = engineManager->engineForName(source);
    if (!engine) {
        qWarning() << "No CSDataEngine supports" << source;
        return;
    }
    // disconnect from current data engine
    if (_engine) {
        _engine->close(this);
        reset();
    }
    _source = source;
    _engine = engine;
    engine->create(this);
    emit sourceChanged();
}

/*!
    \property QCSData::host
    This property holds the host name where the data source exists.
*/
/*!
    \qmlproperty string CSData::host
    This property holds the host name where the data source exists.
*/
QString QCSData::host() const
{
    return _host;
}
void QCSData::setHost(const QString host)
{
    if (_host == host)
        return;
    _host = host;
    emit hostChanged();
}

/*!
    \property QCSData::connected
    This property indicates wether the data is connected to a valid source.
*/
/*!
    \qmlproperty bool CSData::connected
    This property indicates wether the data is connected to a valid source.
*/
bool QCSData::connected() const
{
    return _connected;
}
void QCSData::setConnected(bool connected)
{
    if (_connected == connected)
        return;
    _connected = connected;
    emit connectionChanged();
}

/*!
    \property QCSData::fieldType
    This property holds native type of the data.
*/
/*!
    \qmlproperty enumeration CSData::fieldType
    This property holds native type of the data.

    \sa QCSData::FieldType
*/
QCSData::FieldType QCSData::fieldType() const
{
    return _fieldType;
}
void QCSData::setFieldType(QCSData::FieldType fieldType)
{
    if (_fieldType == fieldType)
        return;
    _fieldType = fieldType;
    emit fieldTypeChanged();
}

/*!
    \property QCSData::count
    This property holds the number of elements of the data.
*/
/*!
    \qmlproperty int CSData::count
    This property holds the number of elements of the data.
*/
qulonglong QCSData::count() const
{
    return _count;
}
void QCSData::setCount(qulonglong count)
{
    if (_count == count)
        return;
    _count = count;
    emit countChanged();
}

/*!
    \property QCSData::accessRight
    This property holds the access right of the data.

    This is a bitwise or of CSData::AccessFlag.
*/
/*!
    \qmlproperty int CSData::accessRight
    This property holds the access right of the data.

    This is a bitwise or of CSData::AccessFlag.
*/
int QCSData::accessRight() const
{
    return _accessRight;
}
void QCSData::setAccessRight(int accessRight)
{
    if (_accessRight == accessRight)
        return;
    _accessRight = accessRight;
    emit accessRightChanged();
}

/*!
    \property QCSData::timeStamp
    This property holds the timestamp of data's last change.
*/
/*!
    \qmlproperty Date CSData::timeStamp
    This property holds the timestamp of data's last change.
*/
QDateTime QCSData::timeStamp() const
{
    return _timeStamp;
}
void QCSData::setTimeStamp(const QDateTime timeStamp)
{
    if (_timeStamp == timeStamp)
        return;
    _timeStamp = timeStamp;
    emit timeStampChanged();
}


/*!
    \property QCSData::precision
    Precision of the data in case of float or double type.
*/
/*!
    \qmlproperty int CSData::precision
    Precision of the data in case of float or double type.
*/
int QCSData::precision() const
{
    return _precision;
}
void QCSData::setPrecision(int precision)
{
    if (_precision == precision)
        return;
    _precision = precision;
    emit precisionChanged();
}

/*!
    \property QCSData::units
    This property holds the physical meaning of the data.
*/
/*!
    \qmlproperty string CSData::units
    This property holds the physical meaning of the data.
*/
QString QCSData::units() const
{
    return _units;
}
void QCSData::setUnits(const QString units)
{
    if (_units == units)
        return;
    _units = units;
    emit unitsChanged();
}

/*!
    \property QCSData::stateStrings
    This property holds the string representation of data's enumerated states.
*/
/*!
    \qmlproperty list CSData::stateStrings
    This property holds the string representation of data's enumerated states.
*/
QStringList QCSData::stateStrings() const
{
    return _stateStrings;
}
void QCSData::setStateStrings(const QStringList stateStrings)
{
    if (_stateStrings == stateStrings)
        return;
    _stateStrings = stateStrings;
    emit stateStringsChanged();
}
/*!
    \property QCSData::alarm
    This property holds the alarm information.
*/
/*!
    \qmlproperty CSDataAlarm CSData::alarm
    This property holds the alarm information.
*/
/*!
    \brief Convenient method to set alarm condition, used by the CSDataEngine.
*/
void QCSData::setAlarm(QCSDataAlarm::Severity severity, int status, const QString message)
{
    _alarm->setAlarm(severity, status, message);
}
/*!
    \property QCSData::range
    This property holds the limits information.

    \sa QCSDataRange
*/
/*!
    \qmlproperty CSDataRange CSData::range
    This property holds the limits information.
*/
/*!
    \brief Convenient method to set data limit, used by the CSDataEngine.
*/
void QCSData::setRange(double lower, double upper)
{
    _range->setRange(lower, upper);
}

/*!
    \property QCSData::value
    This property holds the value of the data.
*/
/*!
    \qmlproperty QVariant CSData::value
    This property holds the value of the data.
*/
QVariant QCSData::value() const
{
    return _value;
}
void QCSData::setValue(const QVariant value)
{
    if (_engine)
        _engine->setValue(this, value);
}

/*!
    \brief Method to update data value, used by the CSDataEngine.
*/
void QCSData::updateValue(const QVariant value)
{
    _value.setValue(value);
    emit valueChanged();
}

/*! \internal */
void QCSData::reset()
{
    _source.clear();
    _host.clear();
    _connected = false;

    _fieldType = Invalid;
    _count = 0;
    _value = QVariant();

    _accessRight = NoAccess;
    _alarm->reset();
    _timeStamp.setMSecsSinceEpoch(0);

    _precision = 0;
    _units.clear();
    _range->reset();

    _stateStrings.clear();
}

