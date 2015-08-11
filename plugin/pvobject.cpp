#include "pvobject.h"

#include <QtDebug>
#include <QStringList>
#include <QTime>
#include <string.h>


/*!
    \qmltype PvObject
    \instantiates QQuickPvObject
    \inqmlmodule PvComponents
    \brief Object provides access to EPICS Process Variables.

    Example:
    \qml
        PvObject {
            id: voltagePV
            channel: 'MTEST:Voltage'
        }
    \endqml
*/

/*!
    \qmlproperty string PvObject::channel

    Name of the PV.
*/

/*!
    \qmlproperty var PvObject::value
    This property holds the PV's value.
*/


/*!
    \class QQuickPvObject
    \inmodule PvComponents
    \brief The QQuickPvObject class provides access to EPICS Process Variables.

    QQuickPvObject derives from QObject and encapsulate the PV access methods.
*/
/*!
    \enum QQuickPvObject::FieldType
        \value String
               Fixed length string of 40 charactors.
        \value Integer
               16 bit integer number
        \value Short
               Same as QQuickPvObject::Integer
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
    \enum QQuickPvObject::Severity
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
    \property QQuickPvObject::value
    This property holds the PV's value.
*/

/*!
    \property QQuickPvObject::connected
    This property indicates wether the PV is connected to server.
*/

/*!
    \qmlproperty enumeration PvObject::severity

    Severity of the PV.
*/

/*!
    \property QQuickPvObject::readable
    This property indicates wether the PV is readable
*/
/*!
    \qmlproperty bool PvObject::readable

    The read access to the PV.
*/

/*!
    \property QQuickPvObject::writable
    This property indicates wether the PV is writable
*/
/*!
    \qmlproperty bool PvObject::writable

    The write access to the PV.
*/

/*!
    \qmlproperty bool PvObject::count

    Element count of the PV.
*/

/*!
    \qmlproperty bool PvObject::units

    Units of the PV.
*/

/*!
    \qmlproperty bool PvObject::prec

    Precision of the PV.
*/

/*!
    \qmlproperty bool PvObject::nostr

    Number of choices of the PV.
*/

/*!
    \qmlproperty bool PvObject::strs

    Choices list of the PV.
*/

/*!
    \qmlmethod PvComponents::PvObject::setValue(var value)

    Set new value to the PV.
*/

struct ca_client_context * QQuickPvObject::m_cac = NULL;

static void connectCallbackC(struct connection_handler_args args);
static void getCallbackC(struct event_handler_args);
static void monitorCallbackC(struct event_handler_args);
static void accessCallbackC(struct access_rights_handler_args args);


/*! \internal */
long QQuickPvObject::init_ca()
{
    int status;

    status = ca_context_create(ca_enable_preemptive_callback);
    if(status != ECA_NORMAL)
        return(CA_EXTRACT_MSG_NO(status));

    m_cac = ca_current_context();

    ca_add_exception_event(exception_handler, 0);

    return 0;
}
/*! \internal */
long QQuickPvObject::exit_ca()
{
    ENTER_CA{
        ca_context_destroy();
        m_cac = NULL;
    }LEAVE_CA

    return 0;
}

void QQuickPvObject::exception_handler(exception_handler_args args)
{
    Q_UNUSED(args);
}

/*!
    Construct a QQuickPvObject with the given \a parent.
 */
QQuickPvObject::QQuickPvObject(QObject *parent):
    QObject(parent)
{
    if (m_cac == NULL)
        init_ca();
    _chid = NULL;
    _evid = NULL;
    _connected = false;
    _status = -1;
    _severity = -1;
    _array = NULL;
    _value = "";
    _name = "";
}

/*!
    \brief Destroy the QQuickPvObject.
    This also disconnects the channel access connections.
*/
QQuickPvObject::~QQuickPvObject()
{
    unsubscribe();
    disconnect();
    if (_array)
        free(_array);
}
/*! \reimp */
void QQuickPvObject::classBegin()
{

}
/*! \reimp */
void QQuickPvObject::componentComplete()
{
    if (_name.isEmpty())
        return;
    connect(_name.toLatin1());
}


void * QQuickPvObject::getArrayValue(unsigned long count)
{
    count  = qMin<unsigned long>(count, ca_element_count(_chid));
    chtype reqtype = dbf_type_to_DBR(ca_field_type(_chid));
    _array = realloc(_array, sizeof(reqtype) * count);
    int status = ca_array_get(reqtype, count, _chid, _array);
    status = ca_pend_io(1);
    if (status != ECA_NORMAL)
        return NULL;
    return _array;
}

void QQuickPvObject::updateValue(const QVariant value)
{
    _value.setValue(value);
    emit valueChanged();
}

void QQuickPvObject::updateStatus(int severity, int status)
{
    if (_status != status || _severity != severity) {
        _status = status;
        _severity = severity;
        emit statusChanged();
    }
}

/*!
*/

QVariant QQuickPvObject::value()
{
    return _value;
}

/*!
 * \brief QQuickPvObject::setValue
 */
void QQuickPvObject::setValue(const QVariant val)
{
    if (!_connected)
        return;

    chtype reqtype = dbf_type_to_DBR(ca_field_type(_chid));
    int status = ECA_NORMAL;
    bool ok = false;

    switch (ca_field_type(_chid)) {
    case DBF_ENUM:
    {
        int value = val.toInt(&ok);
        if (ok)
            status = ca_array_put(DBR_LONG, 1, _chid, &value);
        else
            status = ca_array_put(DBR_STRING, 1, _chid, val.toString().toLatin1());
    }
    break;
    case DBF_CHAR:
    {
        QByteArray ba = val.toByteArray();
        ca_array_put(DBR_CHAR, ba.length(), _chid, ba);
    }
    break;
    case DBF_INT:
    case DBF_LONG:
    {
        int value = val.toInt(&ok);
        if (ok)
            status = ca_array_put(DBR_LONG, 1, _chid, &value);
    }
    break;
    case DBF_FLOAT:
    case DBF_DOUBLE:
    {
        double value = val.toDouble(&ok);
        if (ok)
            status = ca_array_put(DBR_DOUBLE, 1, _chid, &value);
    }
    break;
    case DBF_STRING:
    {
        QString str = val.toString();
        status = ca_array_put(reqtype, 1, _chid, str.toLatin1());
    }
    break;
    default:
        break;
    }
    if (status != ECA_NORMAL)
        qDebug() << ca_message(status);
    ca_flush_io();
}

/*! \internal */
void QQuickPvObject::updateAccess(bool read, bool write)
{
    _readable = read;
    _writable = write;
}

/*! \internal */
long QQuickPvObject::connect(const char* name)
{
    int status;

    ENTER_CA{
        // make connection
        status = ca_create_channel(name, connectCallbackC, this, 0, &_chid);
        if(status != ECA_NORMAL)
        {
            ca_clear_channel(_chid);
            _chid = 0;
            return(CA_EXTRACT_MSG_NO(status));
        }
    }LEAVE_CA

    return 0;
}
/*! \internal */
long QQuickPvObject::disconnect()
{
    if(_chid)
    {
        ENTER_CA{
            int status = ca_clear_channel(_chid);
            _chid = NULL;
            if(status != ECA_NORMAL)
                return(CA_EXTRACT_MSG_NO(status));
        }LEAVE_CA
    }
    return 0;
}

/*! \internal */
long QQuickPvObject::subscribe(unsigned long count)
{
    int status;

    if(_chid && !_evid)
    {
        chtype reqtype = dbf_type_to_DBR_TIME(ca_field_type(_chid));
        ENTER_CA{
            status = ca_create_subscription(reqtype,
                count,
                _chid,
                DBE_VALUE | DBE_ALARM,
                monitorCallbackC,
                this,
                &_evid);
            ca_flush_io();
            if(status != ECA_NORMAL)
                return(CA_EXTRACT_MSG_NO(status));
        }LEAVE_CA
    }
    return 0;
}

/*! \internal */
long QQuickPvObject::unsubscribe()
{
    int status;

    if(_evid)
    {
        ENTER_CA{
            status = ca_clear_subscription(_evid);
            _evid = 0;
            if(status != ECA_NORMAL)
                return(CA_EXTRACT_MSG_NO(status));
        }LEAVE_CA
    }
    return 0;
}

#define ConvertSTS(VP) \
    status      = VP.status;\
    severity	= VP.severity;

#define ConvertTime(VP) \
    _sec		= VP.stamp.secPastEpoch;\
    _nsec		= VP.stamp.nsec;\

#define ConvertCtrl(VP) \
    _units		= VP.units;\
    _updisplim = VP.upper_disp_limit;\
    _lodisplim = VP.lower_disp_limit;\
    _upwarnlim = VP.upper_warning_limit;\
    _lowarnlim = VP.lower_warning_limit;\
    _upalrmlim = VP.upper_alarm_limit;\
    _loalrmlim = VP.lower_alarm_limit;\
    _upctrllim = VP.upper_ctrl_limit;\
    _loctrllim = VP.lower_ctrl_limit;


#define ConvertValue(VP,TYPE)\
    if (count==1) {\
        value.setValue((TYPE)(VP.value));\
    } else {\
        QList<qreal> list;\
        for(unsigned long i=0; i<count; i++) {\
            TYPE v = (TYPE)*(&(VP.value) + i); \
            list.append(v);\
        }\
        value.setValue(QVariant::fromValue(list));\
    }
typedef dbr_string_t* dbr_string_t_ptr;


//
// Connection Callback
//
void connectCallbackC(struct connection_handler_args args)
{
    QQuickPvObject *chan = (QQuickPvObject *)ca_puser(args.chid);
    chan->connectCallback(args);
}
void QQuickPvObject::connectCallback(struct connection_handler_args args)
{
    // Do a get when a channel gets connected
    if (args.op == CA_OP_CONN_UP) {
        // Initialize PV info by CA  get
        chtype reqtype = dbf_type_to_DBR_CTRL(ca_field_type(_chid));
        int status = ca_array_get_callback(reqtype, 0, _chid, getCallbackC, this);
        if (status != ECA_NORMAL)
            return;
        // Add access right handler
        status = ca_replace_access_rights_event(_chid, accessCallbackC);
        if (status != ECA_NORMAL)
            return;
        ca_flush_io();
    } else {
        _connected = false;
        emit connectionChanged();
    }
}

void accessCallbackC(struct access_rights_handler_args args)
{
    QQuickPvObject *chan = (QQuickPvObject *)ca_puser(args.chid);
    chan->updateAccess(args.ar.read_access, args.ar.write_access);
}

//
// Connection Callback
//
void getCallbackC(struct event_handler_args args)
{
    if (args.status != ECA_NORMAL)
        return;
    QQuickPvObject *chan = (QQuickPvObject *)args.usr;
    chan->getCallback(args);
    if (chan->monitor())
        chan->subscribe();
}
void QQuickPvObject::getCallback(struct event_handler_args args)
{
    union db_access_val *val = (union db_access_val *)args.dbr;
    chtype type = args.type;
    unsigned long count  = args.count;
    _count = count;

    int status, severity;
    QVariant value;
    QStringList strList;

    switch (type)
    {
    case DBR_CTRL_STRING:
        ConvertSTS(val->tshrtval)
        ConvertTime(val->tshrtval)
        for(unsigned long i=0; i<count; i++)
            strList.push_back(*(dbr_string_t_ptr)(&(val->tstrval.value) + i));
        value.setValue(strList);
        break;
    case DBR_CTRL_INT:
        ConvertSTS(val->cshrtval)
        ConvertCtrl(val->cshrtval)
        ConvertValue(val->cshrtval,dbr_int_t)
        break;
    case DBR_CTRL_FLOAT:
        ConvertSTS(val->cfltval);
        ConvertCtrl(val->cfltval);
        ConvertValue(val->cfltval,dbr_float_t)
        _precision = val->cfltval.precision;
        break;
    case DBR_CTRL_ENUM:
        ConvertSTS(val->cenmval);
        _nostr = val->cenmval.no_str;
        _strs.clear();
        for(int i=0; i<val->cenmval.no_str; i++)
            _strs.append(val->cenmval.strs[i]);
        ConvertValue(val->cenmval,dbr_enum_t)
        break;
    case DBR_CTRL_CHAR:
        ConvertSTS(val->cchrval);
        ConvertCtrl(val->cchrval);
        ConvertValue(val->cchrval,dbr_char_t)
        break;
    case DBR_CTRL_LONG:
        ConvertSTS(val->clngval);
        ConvertCtrl(val->clngval);
        ConvertValue(val->clngval,dbr_long_t)
        break;
    case DBR_CTRL_DOUBLE:
        ConvertSTS(val->cdblval);
        ConvertCtrl(val->cdblval);
        ConvertValue(val->cdblval,dbr_double_t)
        _precision = val->cdblval.precision;
        break;
    }

    // Set connected after first get succeeds
    _connected = true;
    emit connectionChanged();
    updateValue(value);
    // Signal status/severity
    updateStatus(severity, status);
}

//
// Monitor Callback
//
void monitorCallbackC(struct event_handler_args args)
{
    if (args.status != ECA_NORMAL)
        return;
    QQuickPvObject *chan = (QQuickPvObject *)args.usr;
    chan->monitorCallback(args);
}
void QQuickPvObject::monitorCallback(struct event_handler_args args)
{
    union db_access_val *val = (union db_access_val *)args.dbr;
    chtype type = args.type;
    unsigned long count  = args.count;

    int status, severity;
    QVariant value;
    QStringList strList;

    switch (type)
    {
    case DBR_TIME_STRING:
        ConvertSTS(val->tshrtval)
        ConvertTime(val->tshrtval)
        for(unsigned long i=0; i<count; i++)
            strList.push_back(*(dbr_string_t_ptr)(&(val->tstrval.value) + i));
        value.setValue(strList);
        break;
    case DBR_TIME_INT:
        ConvertSTS(val->tshrtval)
        ConvertTime(val->tshrtval)
        ConvertValue(val->tshrtval,dbr_int_t)
        break;
    case DBR_TIME_FLOAT:
        ConvertSTS(val->tfltval);
        ConvertTime(val->tfltval);
        ConvertValue(val->tfltval,dbr_float_t)
        break;
    case DBR_TIME_ENUM:
        ConvertSTS(val->tenmval);
        ConvertTime(val->tenmval);
        ConvertValue(val->tenmval,dbr_enum_t)
        break;
    case DBR_TIME_CHAR:
        ConvertSTS(val->tchrval);
        ConvertTime(val->tchrval);
        ConvertValue(val->tchrval,dbr_char_t)
        break;
    case DBR_TIME_LONG:
        ConvertSTS(val->tlngval);
        ConvertTime(val->tlngval);
        ConvertValue(val->tlngval,dbr_long_t)
        break;
    case DBR_TIME_DOUBLE:
        ConvertSTS(val->tdblval);
        ConvertTime(val->tdblval);
        ConvertValue(val->tdblval,dbr_double_t)
        break;
    }
    // Signal value change
    updateValue(value);
    // Signal status/severity change if any
    updateStatus(severity, status);
}
