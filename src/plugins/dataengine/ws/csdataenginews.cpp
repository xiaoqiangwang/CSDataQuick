/*****************************************************************************\
 * Copyright 2012-2022 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/
#include <cstdlib>

#include "csdataenginews.h"
#include "csdata.h"
#include "objectmodel.h"

#include <QtDebug>
#include <QWebSocket>
#include <QUrl>
#include <QCborValue>
#include <QCborArray>
#include <QCborMap>
#include <QCborStreamWriter>
#include <QCborStreamReader>

/*!
 * \class QCSDataEngineWS
 * \inmodule CSDataQuick.Data
 * \ingroup csdataquick.data.plugins
 * \brief Get data via a WebSocket server.
 *
 * This data engine source has a \e ws:// scheme, followed by the data name.
 */
QCSDataEngineWS::QCSDataEngineWS(QObject *parent)
    : QCSDataEngine(parent), _connected(false), _cid(0)
{
    _data = new ObjectModel(this);
    _ws = new QWebSocket();
    _ws->open(QUrl("ws://127.0.0.1:50064/api/csdata"));
    connect(_ws, &QWebSocket::binaryMessageReceived, this, &QCSDataEngineWS::ws_message);
    connect(_ws, &QWebSocket::connected, this, &QCSDataEngineWS::ws_connected);
    connect(_ws, &QWebSocket::disconnected, this, &QCSDataEngineWS::ws_disconnected);

    QByteArrayList roles;
    roles << "source" << "connected";
    _data->setup<QCSData>(roles);
}

QCSDataEngineWS::~QCSDataEngineWS()
{
}

/*!
 * \reimp
 * \brief Returns the source scheme \e ws.
 */
QString QCSDataEngineWS::name()
{
    return "ws";
}

/*!
 * \reimp
 */
QString QCSDataEngineWS::description()
{
    return "WebSocket Engine";
}

/*!
 * \reimp
 */
void QCSDataEngineWS::create(QCSData *data)
{
    _cid++;
    data->setProperty("cid", _cid);

    if (_connected) {
        QCborValue c;
        c["command"] = "subscribe";
        c["name"] = data->property("source").toString();
        c["cid"] = _cid;

        QByteArray message;
        QCborStreamWriter writer(&message);
        c.toCbor(writer);

        _ws->sendBinaryMessage(message);
    } else
        _pending.append(data);

    _data->append(data);
    _table[_cid] = data;
    emit allDataChanged();
}

/*!
 * \reimp
 */
void QCSDataEngineWS::close(QCSData *data)
{
    double cid = data->property("cid").toDouble();

    QCborValue c;
    c["command"] = "unsubscribe";
    c["name"] = data->property("source").toString();
    c["cid"] = cid;

    QByteArray message;
    QCborStreamWriter writer(&message);
    c.toCbor(writer);

    _ws->sendBinaryMessage(message);

    _table.remove(cid);
    _data->remove(data);

    emit allDataChanged();
}

/*!
 * \reimp
 */
void QCSDataEngineWS::setValue(QCSData *data, const QVariant value)
{
    if (!_data->contains(data))
        return;

    double cid = data->property("cid").toDouble();

    QCborValue c;
    c["command"] = "put";
    c["name"] = data->property("source").toString();
    if (value.canConvert<QVariantList>()) {
        c["value"] = QCborArray::fromVariantList(value.toList());
    } else {
        c["value"] = QCborValue::fromVariant(value);
    }
    c["cid"] = cid;

    QByteArray message;
    QCborStreamWriter writer(&message);
    c.toCbor(writer);

    _ws->sendBinaryMessage(message);
}

/*!
 * \reimp
 */
ObjectModel *QCSDataEngineWS::allData()
{
    return _data;
}

void QCSDataEngineWS::ws_message(const QByteArray &message)
{
    QCborStreamReader reader(message);
    QCborMap m = QCborValue::fromCbor(reader).toMap();
    double cid = m[QLatin1String("cid")].toDouble();
    QCSData *data = _table[cid];
    if (m.contains(QLatin1String("connection"))) {
        QMetaObject::invokeMethod(data, "setConnected", Q_ARG(bool, m[QLatin1String("connection")].toBool()));
    } else {
        QCSData::FieldType ftype = QCSData::Invalid;
        switch (m[QLatin1String("ftype")].toInteger()) {
            case 0: ftype = QCSData::String; break;
            case 4: ftype = QCSData::Char;   break;
            case 3: ftype = QCSData::Enum;   break;
            case 1: ftype = QCSData::Short;  break;
            case 5: ftype = QCSData::Integer;break;
            case 2: ftype = QCSData::Float;  break;
            case 6: ftype = QCSData::Double; break;
        }
        QMetaObject::invokeMethod(data, "setFieldType",
                                  Q_ARG(QCSData::FieldType, ftype));
        QMetaObject::invokeMethod(data, "setCount", Q_ARG(qulonglong, m[QLatin1String("count")].toInteger()));

        QMetaObject::invokeMethod(data, "setAccessRight", Q_ARG(int, (int)(QCSData::ReadAccess | QCSData::WriteAccess)));
        QMetaObject::invokeMethod(data, "setUnits", Q_ARG(QString, m[QLatin1String("display")][QLatin1String("units")].toString()));
        QMetaObject::invokeMethod(data, "setPrecision", Q_ARG(int, m[QLatin1String("display")][QLatin1String("precision")].toInteger()));
        QMetaObject::invokeMethod(data, "setRange",
                                  Q_ARG(double, m[QLatin1String("range")].toArray()[0].toDouble()),
                                  Q_ARG(double, m[QLatin1String("range")].toArray()[1].toDouble())
        ); 
        QMetaObject::invokeMethod(data, "setAlarm",
                                  Q_ARG(QCSDataAlarm::Severity, (QCSDataAlarm::Severity)m[QLatin1String("alarm")][QLatin1String("severity")].toInteger()),
                                  Q_ARG(int, m[QLatin1String("alarm")][QLatin1String("status")].toInteger()),
                                  Q_ARG(QString, m[QLatin1String("alarm")][QLatin1String("message")].toString()));
        QDateTime timeStamp;
        timeStamp.setMSecsSinceEpoch(
            m[QLatin1String("timeStamp")][QLatin1String("secondsPastEpoch")].toInteger() * 1000 +
            qint64( m[QLatin1String("timeStamp")][QLatin1String("nanoseconds")].toInteger() / 1000000)
        );
        QMetaObject::invokeMethod(data, "setTimeStamp", Q_ARG(QDateTime, timeStamp));

        if (m[QLatin1String("value")].isMap()) {
            int index = m[QLatin1String("value")][QLatin1String("index")].toInteger();
            QStringList choices;
            for (auto c: m[QLatin1String("value")][QLatin1String("choices")].toArray())
                choices.append(c.toString());
            QMetaObject::invokeMethod(data, "updateValue", Q_ARG(QVariant, index));
            QMetaObject::invokeMethod(data, "setStateStrings", Q_ARG(QStringList, choices));
        } else {
            QMetaObject::invokeMethod(data, "updateValue", Q_ARG(QVariant, m[QLatin1String("value")].toVariant()));
        }
    }
}

void QCSDataEngineWS::ws_connected()
{
    _connected = true;

    for(int i=0; i<_pending.size(); i++) {
        QCborValue c;
        c["command"] = "subscribe";
        c["name"] = _data->at(i)->property("source").toString();
        c["cid"] = _data->at(i)->property("cid").toDouble();

        QByteArray message;
        QCborStreamWriter writer(&message);
        c.toCbor(writer);

        _ws->sendBinaryMessage(message);
    }
    _pending.clear();
}

void QCSDataEngineWS::ws_disconnected()
{
    _connected = false;

    for(int i=0; i<_data->size(); i++) {
        _data->at(i)->setProperty("connected", false);
    }
}
