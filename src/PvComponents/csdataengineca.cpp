#include "csdataengineca.h"
#include "csdata.h"

// epics base headers
#include <cadef.h>
#define epicsAlarmGLOBAL
#include <alarm.h>

#include <QtDebug>

void exception_handler(exception_handler_args args)
{
    qWarning(
      "PvObject: Channel Access Exception:\n"
      "  Channel Name: %s\n"
      "  Native Type: %s\n"
      "  Native Count: %lu\n"
      "  Access: %s%s\n"
      "  Host: %s\n"
      "  Message: %s\n"
      "  Context: %s\n"
      "  Requested Type: %s\n"
      "  Requested Count: %ld\n"
      "  Source File: %s\n"
      "  Line number: %u\n",
      args.chid?ca_name(args.chid):"Unavailable",
      args.chid?dbf_type_to_text(ca_field_type(args.chid)):"Unavailable",
      args.chid?ca_element_count(args.chid):0,
      args.chid?(ca_read_access(args.chid)?"R":"None"):"Unavailable",
      args.chid?(ca_write_access(args.chid)?"W":""):"",
      args.chid?ca_host_name(args.chid):"Unavailable",
      ca_message(args.stat)?ca_message(args.stat):"Unavailable",
      args.ctx?args.ctx:"Unavailable",
      dbf_type_to_text(args.type),
      args.count,
      args.pFile?args.pFile:"Unavailable",
      args.pFile?args.lineNo:0);
}

//
// Get Callback
//

#define ConvertSTS(VP) \
    status      = VP.status;\
    severity	= VP.severity;

#define ConvertTime(VP) \
    timeStamp.setMSecsSinceEpoch(qint64(VP.stamp.secPastEpoch + POSIX_TIME_AT_EPICS_EPOCH) * 1000 + qint64(VP.stamp.nsec / 1000));

#define ConvertCtrl(VP) \
    QMetaObject::invokeMethod(data, "setUnits", Q_ARG(QString, VP.units));\
    QMetaObject::invokeMethod(data, "setRange", Q_ARG(double, VP.lower_disp_limit),\
                                                Q_ARG(double, VP.upper_disp_limit));

#define ConvertValue(VP,TYPE)\
    if (count==1) {\
        value.setValue((TYPE)(VP.value));\
    } else {\
        QList<qreal> list;\
        for(qulonglong i=0; i<count; i++) {\
            TYPE v = (TYPE)*(&(VP.value) + i); \
            list.append(v);\
        }\
        value.setValue(QVariant::fromValue(list));\
    }
typedef dbr_string_t* dbr_string_t_ptr;

//
// Monitor Callback
//
void monitorCallbackC(struct event_handler_args args)
{
    if (args.status != ECA_NORMAL)
        return;
    CSData *data = (CSData *)args.usr;

    union db_access_val *val = (union db_access_val *)args.dbr;
    chtype type = args.type;
    qulonglong count  = args.count;

    int status, severity;
    QDateTime timeStamp;
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
    QMetaObject::invokeMethod(data, "updateValue", Q_ARG(QVariant, value));
    QMetaObject::invokeMethod(data, "setAlarm",
                              Q_ARG(CSDataAlarm::Severity, (CSDataAlarm::Severity)severity),
                              Q_ARG(int, status),
                              Q_ARG(QString, epicsAlarmConditionStrings[status]));
    QMetaObject::invokeMethod(data, "setTimeStamp",
                              Q_ARG(QDateTime, timeStamp));
}

void getCallbackC(struct event_handler_args args)
{
    if (args.status != ECA_NORMAL) {
        qWarning() << "getCallbackC" << ca_message(args.status);
        return;
    }

    CSData *data = (CSData *)args.usr;

    union db_access_val *val = (union db_access_val *)args.dbr;
    chtype type = args.type;
    qulonglong count  = args.count;
    QMetaObject::invokeMethod(data, "setCount", Q_ARG(qulonglong, count));

    int status, severity;
    QDateTime timeStamp;
    QVariant value;
    QStringList strList;

    switch (type)
    {
    case DBR_CTRL_STRING:
        ConvertSTS(val->tshrtval)
        ConvertTime(val->tshrtval)
        for(qulonglong i=0; i<count; i++)
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
        QMetaObject::invokeMethod(data, "setPrecision", Q_ARG(int, val->cfltval.precision));
        break;
    case DBR_CTRL_ENUM:
        ConvertSTS(val->cenmval);
        for(int i=0; i<val->cenmval.no_str; i++)
            strList.append(val->cenmval.strs[i]);
        QMetaObject::invokeMethod(data, "setStateStrings", Q_ARG(QStringList, strList));
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
        QMetaObject::invokeMethod(data, "setPrecision", Q_ARG(int, val->cdblval.precision));
        break;
    }

    // Set connected after first get succeeds
    QMetaObject::invokeMethod(data, "setConnected", Q_ARG(bool, true));

    // Subscribe to value and alarm changes
    evid evid;
    chtype reqtype = dbf_type_to_DBR_TIME(ca_field_type(args.chid));
    status = ca_create_subscription(reqtype,
                                    count,
                                    args.chid,
                                    DBE_VALUE | DBE_ALARM,
                                    monitorCallbackC,
                                    data,
                                    &evid);
    ca_flush_io();
    if(status != ECA_NORMAL)
        qCritical() << "ca_create_subscription:" << ca_message(status);
}

void accessCallbackC(struct access_rights_handler_args args)
{
    CSData *data = (CSData *)ca_puser(args.chid);
    CSData::AccessFlags accessRight = (args.ar.read_access ? CSData::ReadAccess : CSData::NoAccess) |
            (args.ar.write_access ? CSData::WriteAccess : CSData::NoAccess);
    QMetaObject::invokeMethod(data, "setAccessRight",
                              Q_ARG(int, (int)accessRight));
}

//
// Connection Callback
//
void connectCallbackC(struct connection_handler_args args)
{
    CSData *data = (CSData *)ca_puser(args.chid);
    // Do a get when a channel gets connected
    if (args.op == CA_OP_CONN_UP) {
        QMetaObject::invokeMethod(data, "setHost",  Q_ARG(QString, ca_host_name(args.chid)));
        QMetaObject::invokeMethod(data, "setFieldType",
                                  Q_ARG(CSData::FieldType, (CSData::FieldType) ca_field_type(args.chid)));
        // Initiate CA get to get control information
        chtype reqtype = dbf_type_to_DBR_CTRL(ca_field_type(args.chid));
        int status = ca_array_get_callback(reqtype, 0, args.chid, getCallbackC, data);
        if (status != ECA_NORMAL) {
            qCritical() << "ca_array_get_callback:" << ca_message(status);
            return;
        }
        // Add access right handler
        status = ca_replace_access_rights_event(args.chid, accessCallbackC);
        if (status != ECA_NORMAL){
            qCritical() << "ca_replace_access_rights_event:" << ca_message(status);
            return;
        }
        ca_flush_io();
    } else {
        QMetaObject::invokeMethod(data, "setConnected", Q_ARG(bool, false));
        QMetaObject::invokeMethod(data, "setFieldType", Q_ARG(CSData::FieldType, CSData::Invalid));
    }
}

CSDataEngineCA::CSDataEngineCA(QObject *parent)
    : QObject(parent)
{
    int status = ca_context_create(ca_enable_preemptive_callback);
    if(status != ECA_NORMAL)
        qCritical() << ca_message(status);

    _cac = ca_current_context();

    ca_add_exception_event(exception_handler, 0);
}

CSDataEngineCA::~CSDataEngineCA()
{
}

QString CSDataEngineCA::name()
{
    return "ca";
}

void CSDataEngineCA::create(CSData *data)
{
    QString source = data->property("source").toString();
    QUrl url(source);

    if (!url.scheme().isEmpty() && url.scheme() != name()) {
        qWarning() << "Unsupported source by CSDataEngineCA" << source;
        return;
    }

    if (ca_current_context() != _cac) {
        ca_attach_context(_cac);
        qWarning() << "Current CA context has been swappped";
    }

    // make connection
    chid _chid = 0;
    QString name = url.host();
    int status = ca_create_channel(source.toLocal8Bit(), connectCallbackC, data, 0, &_chid);
    if(status != ECA_NORMAL)
    {
        if (_chid) ca_clear_channel(_chid);
        qCritical() << "ca_create_channel:" << source << ca_message(status);
    } else {
        // create a dynamic property to hold the channel id
        data->setProperty("chid", QVariant::fromValue((void*)_chid));
    }
}
void CSDataEngineCA::close(CSData *data)
{
    if (ca_current_context() != _cac) {
        ca_attach_context(_cac);
        qWarning() << "Current CA context has been swappped";
    }

    chid _chid = (chid)data->property("chid").value<void*>();
    int status = ca_clear_channel(_chid);
    if(status != ECA_NORMAL)
        qWarning() << "ca_clear_channel:" << ca_message(status);
    data->setProperty("chid", QVariant::fromValue(Q_NULLPTR));
}

void CSDataEngineCA::setValue(CSData *data, const QVariant value)
{
    if (ca_current_context() != _cac) {
        ca_attach_context(_cac);
        qWarning() << "Current CA context has been swappped";
    }

    chid _chid = (chid)data->property("chid").value<void*>();
    chtype reqtype = dbf_type_to_DBR(ca_field_type(_chid));
    int status = ECA_NORMAL;
    bool ok = false;

    switch (ca_field_type(_chid)) {
    case DBF_ENUM:
    {
        int ivalue = value.toInt(&ok);
        if (ok)
            status = ca_array_put(DBR_LONG, 1, _chid, &ivalue);
        else
            status = ca_array_put(DBR_STRING, 1, _chid, value.toString().toLatin1());
    }
    break;
    case DBF_CHAR:
    {
        QByteArray ba = value.toByteArray();
        ca_array_put(DBR_CHAR, ba.length(), _chid, ba);
    }
    break;
    case DBF_INT:
    case DBF_LONG:
    {
        int ivalue = value.toInt(&ok);
        if (ok)
            status = ca_array_put(DBR_LONG, 1, _chid, &ivalue);
    }
    break;
    case DBF_FLOAT:
    case DBF_DOUBLE:
    {
        double fvalue = value.toDouble(&ok);
        if (ok)
            status = ca_array_put(DBR_DOUBLE, 1, _chid, &fvalue);
    }
    break;
    case DBF_STRING:
    {
        QString str = value.toString();
        status = ca_array_put(reqtype, 1, _chid, str.toLatin1());
    }
    break;
    default:
        break;
    }
    if (status != ECA_NORMAL) {
        qWarning() << data->source() << ca_message(status);
    }
    ca_flush_io();
}
