#include "csdataengineca.h"
#include "csdata.h"
#include "objectmodel.h"

// epics base headers
#include <cadef.h>
#define epicsAlarmGLOBAL
#include <alarm.h>

#include <QTimer>
#include <QtDebug>

void exception_handler(exception_handler_args args)
{
    qWarning(
      "CSDataEngineCA: Channel Access Exception:\n"
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

#define ConvertValue(VP,TYPE,VTYPE)\
    if (element_count==1) {\
        value.setValue((VTYPE)(VP.value));\
    } else {\
        if (use_native_type || sizeof(TYPE) == sizeof(VTYPE)) { \
            QVector<TYPE> list(count);\
            memcpy(list.data(), &VP.value, count * sizeof(TYPE));\
            value.setValue(list);\
        } else {\
            QVector<VTYPE> list(count);\
            for(qulonglong i=0; i<count; i++) {\
                list[i] = (TYPE)*(&(VP.value) + i); \
            }\
            value.setValue(list);\
        }\
    }
typedef dbr_string_t* dbr_string_t_ptr;

//
// Monitor Callback
//
void monitorCallbackC(struct event_handler_args args)
{
    if (args.status != ECA_NORMAL) {
        qWarning() << "monitorCallbackC:" << ca_name(args.chid) << ca_message(args.status);
        return;
    }
    QCSData *data = (QCSData *)args.usr;
    bool use_native_type = data->extraProperties().value("UseNativeType", QVariant(false)).toBool();

    union db_access_val *val = (union db_access_val *)args.dbr;
    chtype type = args.type;
    qulonglong count  = args.count;
    qulonglong element_count  = ca_element_count(args.chid);

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
    case DBR_TIME_SHORT:
        ConvertSTS(val->tshrtval)
        ConvertTime(val->tshrtval)
        ConvertValue(val->tshrtval,dbr_int_t,int)
        break;
    case DBR_TIME_FLOAT:
        ConvertSTS(val->tfltval);
        ConvertTime(val->tfltval);
        ConvertValue(val->tfltval,dbr_float_t,double)
        break;
    case DBR_TIME_ENUM:
        ConvertSTS(val->tenmval);
        ConvertTime(val->tenmval);
        ConvertValue(val->tenmval,dbr_enum_t,int)
        break;
    case DBR_TIME_CHAR:
        ConvertSTS(val->tchrval);
        ConvertTime(val->tchrval);
        ConvertValue(val->tchrval,dbr_char_t,int)
        break;
    case DBR_TIME_LONG:
        ConvertSTS(val->tlngval);
        ConvertTime(val->tlngval);
        ConvertValue(val->tlngval,dbr_long_t,int)
        break;
    case DBR_TIME_DOUBLE:
        ConvertSTS(val->tdblval);
        ConvertTime(val->tdblval);
        ConvertValue(val->tdblval,dbr_double_t,double)
        break;
    }

    QMetaObject::invokeMethod(data, "updateValue", Q_ARG(QVariant, value));
    QMetaObject::invokeMethod(data, "setAlarm",
                              Q_ARG(QCSDataAlarm::Severity, (QCSDataAlarm::Severity)severity),
                              Q_ARG(int, status),
                              Q_ARG(QString, epicsAlarmConditionStrings[status]));
    QMetaObject::invokeMethod(data, "setTimeStamp",
                              Q_ARG(QDateTime, timeStamp));
}

void propertyCallbackC(struct event_handler_args args)
{
    if (args.status != ECA_NORMAL) {
        qWarning() << "propertyCallbackC:" << ca_name(args.chid) << ca_message(args.status);
        return;
    }

    QCSData *data = (QCSData *)args.usr;
    bool use_native_type = data->extraProperties().value("UseNativeType", QVariant(false)).toBool();

    union db_access_val *val = (union db_access_val *)args.dbr;
    chtype type = args.type;
    qulonglong count  = args.count;
    qulonglong element_count  = ca_element_count(args.chid);

    int status, severity;
    QVariant value;
    QStringList strList;

    switch (type)
    {
    case DBR_CTRL_STRING:
        ConvertSTS(val->cstrval)
        for(qulonglong i=0; i<count; i++)
            strList.push_back(*(dbr_string_t_ptr)(&(val->cstrval.value) + i));
        value.setValue(strList);
        break;
    case DBR_CTRL_SHORT:
        ConvertSTS(val->cshrtval)
        ConvertCtrl(val->cshrtval)
        ConvertValue(val->cshrtval,dbr_int_t,int)
        break;
    case DBR_CTRL_FLOAT:
        ConvertSTS(val->cfltval);
        ConvertCtrl(val->cfltval);
        ConvertValue(val->cfltval,dbr_float_t,double)
        QMetaObject::invokeMethod(data, "setPrecision", Q_ARG(int, val->cfltval.precision));
        break;
    case DBR_CTRL_ENUM:
        ConvertSTS(val->cenmval);
        for(int i=0; i<val->cenmval.no_str; i++)
            strList.append(val->cenmval.strs[i]);
        QMetaObject::invokeMethod(data, "setStateStrings", Q_ARG(QStringList, strList));
        ConvertValue(val->cenmval,dbr_enum_t,int)
        break;
    case DBR_CTRL_CHAR:
        ConvertSTS(val->cchrval);
        ConvertCtrl(val->cchrval);
        ConvertValue(val->cchrval,dbr_char_t,int)
        break;
    case DBR_CTRL_LONG:
        ConvertSTS(val->clngval);
        ConvertCtrl(val->clngval);
        ConvertValue(val->clngval,dbr_long_t,int)
        break;
    case DBR_CTRL_DOUBLE:
        ConvertSTS(val->cdblval);
        ConvertCtrl(val->cdblval);
        ConvertValue(val->cdblval,dbr_double_t,double)
        QMetaObject::invokeMethod(data, "setPrecision", Q_ARG(int, val->cdblval.precision));
        break;
    }

    // Subscribe to value and alarm changes
    QVariant v = data->property("evidMonitor");
    if (!v.isValid()) {
        // Set connected after first get succeeds
        QMetaObject::invokeMethod(data, "setConnected", Q_ARG(bool, true));

        evid evidMonitor;
        chtype reqtype = dbf_type_to_DBR_TIME(ca_field_type(args.chid));
        status = ca_create_subscription(reqtype,
                                        0,
                                        args.chid,
                                        DBE_VALUE | DBE_ALARM,
                                        monitorCallbackC,
                                        data,
                                        &evidMonitor);
        if(status != ECA_NORMAL)
            qCritical() << "ca_create_subscription:" << ca_name(args.chid) << ca_message(status);
        else {
            data->setProperty("evidMonitor", QVariant::fromValue((void*)evidMonitor));
            ca_flush_io();
        }
    }
}

void accessCallbackC(struct access_rights_handler_args args)
{
    QCSData *data = (QCSData *)ca_puser(args.chid);
    QCSData::AccessFlags accessRight = (args.ar.read_access ? QCSData::ReadAccess : QCSData::NoAccess) |
            (args.ar.write_access ? QCSData::WriteAccess : QCSData::NoAccess);
    QMetaObject::invokeMethod(data, "setAccessRight",
                              Q_ARG(int, (int)accessRight));
}

//
// Connection Callback
//
void connectCallbackC(struct connection_handler_args args)
{
    QCSData *data = (QCSData *)ca_puser(args.chid);
    int status;
    if (args.op == CA_OP_CONN_UP) {
        QMetaObject::invokeMethod(data, "setHost",  Q_ARG(QString, ca_host_name(args.chid)));
        QMetaObject::invokeMethod(data, "setFieldType",
                                  Q_ARG(QCSData::FieldType, (QCSData::FieldType) ca_field_type(args.chid)));
        QMetaObject::invokeMethod(data, "setCount", Q_ARG(qulonglong, ca_element_count(args.chid)));

        // Add property change monitor
        QVariant v = data->property("evidProperty");
        if (!v.isValid()) {
            evid evidProperty;
            chtype reqtype = dbf_type_to_DBR_CTRL(ca_field_type(args.chid));
            status = ca_create_subscription(reqtype, 0, args.chid, DBE_PROPERTY, propertyCallbackC, data, &evidProperty);
            if (status != ECA_NORMAL) {
                qCritical() << "ca_array_get_callback:" << ca_message(status);
                return;
            }
            data->setProperty("evidProperty", QVariant::fromValue((void*)evidProperty));
        }
        // Replace access right handler
        status = ca_replace_access_rights_event(args.chid, accessCallbackC);
        if (status != ECA_NORMAL){
            qCritical() << "ca_replace_access_rights_event:" << ca_message(status);
            return;
        }
        ca_flush_io();
    } else {
        QMetaObject::invokeMethod(data, "setConnected", Q_ARG(bool, false));
        QMetaObject::invokeMethod(data, "setFieldType", Q_ARG(QCSData::FieldType, QCSData::Invalid));
    }
}

QCSDataEngineCA::QCSDataEngineCA(QObject *parent)
    : QCSDataEngine(parent)
{
    _data = new ObjectModel(this);
    QByteArrayList roles;
    roles << "source" << "connected";
    _data->setup<QCSData>(roles);

    _update_scheduled = false;

    int status = ca_context_create(ca_enable_preemptive_callback);
    if(status != ECA_NORMAL)
        qCritical() << "ca_context_create:" << ca_message(status);

    _cac = ca_current_context();

    ca_add_exception_event(exception_handler, 0);
}

QCSDataEngineCA::~QCSDataEngineCA()
{
}

QString QCSDataEngineCA::name()
{
    return "ca";
}

QString QCSDataEngineCA::description()
{
    return "Channel Access Engine";
}

void QCSDataEngineCA::create(QCSData *data)
{
    QCSDataSourceName source = QCSDataSourceName(data->property("source").toString());

    if (!source.scheme().isEmpty() && source.scheme() != name()) {
        qWarning() << "Unsupported source by CSDataEngineCA" << source.source();
        return;
    }


    if (ca_current_context() != _cac) {
        ca_attach_context(_cac);
        qWarning() << "Current CA context has been swappped";
    }

    // make connection
    QString channelName = source.name();
    chid _chid = 0;
    int status = ca_create_channel(channelName.toLocal8Bit(), connectCallbackC, data, 0, &_chid);
    if(status != ECA_NORMAL)
    {
        if (_chid) ca_clear_channel(_chid);
        qCritical() << "ca_create_channel:" << channelName << ca_message(status);
    } else {
        // create a dynamic property to hold the channel id
        data->setProperty("chid", QVariant::fromValue((void*)_chid));
        _data->append(data);
        dataChanged();
    }

    ca_flush_io();
}
void QCSDataEngineCA::close(QCSData *data)
{
    if (ca_current_context() != _cac) {
        ca_attach_context(_cac);
        qWarning() << "Current CA context has been swappped";
    }
    // Clear channel
    chid _chid = (chid)data->property("chid").value<void*>();
    int status = ca_clear_channel(_chid);
    if(status != ECA_NORMAL)
        qWarning() << "ca_clear_channel:" << ca_message(status);

    // Remove associated dynamic properties
    data->setProperty("chid", QVariant());
    data->setProperty("evidProperty", QVariant());
    data->setProperty("evidMonitor", QVariant());

    ca_flush_io();

    _data->remove(data);
    dataChanged();
}

template <typename T>
QVector<T> setup_put(QVariant value)
{
    QVector<T> val;
    if (value.canConvert<QVariantList>()) {
        foreach(QVariant v, value.value<QSequentialIterable>()) {
            if (v.isValid())
                val.append(v.value<T>());
            else
                val.append(0);
        }
    } else {
        val.append(value.value<T>());
    }
    return val;
}

void QCSDataEngineCA::setValue(QCSData *data, const QVariant value)
{
    if (ca_current_context() != _cac) {
        ca_attach_context(_cac);
        qWarning() << "Current CA context has been swappped";
    }

    chid _chid = (chid)data->property("chid").value<void*>();
    chtype reqtype = dbf_type_to_DBR(ca_field_type(_chid));
    int status = ECA_NORMAL;
    unsigned long count = 1;
    qulonglong element_count  = ca_element_count(_chid);

    QVariant newValue(value);
    if (newValue.type() == QVariant::String || newValue.type() == QVariant::ByteArray) {
        if (reqtype == DBR_ENUM || reqtype == DBR_STRING) {
            reqtype = DBR_STRING;
        } else if (element_count == 1) {
            newValue.setValue(value.toDouble());
        } else {
            QByteArray ba = newValue.toByteArray();
            if (ba.isEmpty())
                ba.append('\x00');
            QVector<char> v;
            for(int i=0; i<ba.length(); i++) {
                v.append(ba.at(i));
            }
            newValue.setValue(QVariant::fromValue(v));
        }
    }
    if (newValue.canConvert<QVariantList>()) {
        unsigned long value_count = newValue.value<QVariantList>().count();
        count = qMin(value_count, ca_element_count(_chid));
        if (count == 1)
            newValue.setValue(newValue.value<QVariantList>().at(0));
    }

    switch (reqtype) {
    case DBR_STRING:
    {
        QStringList strs = newValue.toStringList();
        dbr_string_t *pbuf = (dbr_string_t *)calloc(strs.count(), sizeof(dbr_string_t));
        for (int i=0; i<strs.count(); i++) {
            QByteArray ba = strs.at(i).toLocal8Bit();
            strncpy(pbuf[i], ba.constData(), ba.size());
        }
        status = ca_array_put(DBR_STRING, qMin((unsigned long)strs.size(), ca_element_count(_chid)), _chid, pbuf);
    }
    break;
    default:
    {
        QVector<double> v = setup_put<double>(newValue);
        status = ca_array_put(DBR_DOUBLE, qMin((unsigned long)v.size(), ca_element_count(_chid)), _chid, v.data());
    }
    break;
    }
    if (status != ECA_NORMAL) {
        qWarning() << "ca_array_put:" << data->source() << ca_message(status);
    }
    ca_flush_io();
}

ObjectModel* QCSDataEngineCA::allData()
{
    return _data;
}

void QCSDataEngineCA::dataChanged()
{
    if (_update_scheduled)
        return;

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now < _last_update + 250) {
        _update_scheduled = true;
        QTimer::singleShot(250, this, SLOT(notifyDataChange()));
    } else {
        _last_update = now;
        emit allDataChanged();
    }
}

void QCSDataEngineCA::notifyDataChange()
{
    _last_update = QDateTime::currentMSecsSinceEpoch();
    _update_scheduled = false;
    emit allDataChanged();
}
