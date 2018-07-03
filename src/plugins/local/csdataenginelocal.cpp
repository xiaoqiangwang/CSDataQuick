#include "csdataenginelocal.h"
#include "csdata.h"
#include "objectmodel.h"

#include <QtDebug>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>

#include <iostream>

QCSDataEngineLocal::QCSDataEngineLocal(QObject *parent)
    : QCSDataEngine(parent)
{
    _data = new ObjectModel(this);

    QByteArrayList roles;
    roles << "source" << "connected";
    _data->setup<QCSData>(roles);
}

QCSDataEngineLocal::~QCSDataEngineLocal()
{
}

QString QCSDataEngineLocal::name()
{
    return "loc";
}

QString QCSDataEngineLocal::description()
{
    return "Local Data Engine";
}

void QCSDataEngineLocal::notifyDataChange(QCSData *data, QString name)
{
    data->updateValue(_objectsMap[name]["value"].toVariant());
    QString type = _objectsMap[name]["type"].toString();
    if (type == "int")
        data->setProperty("fieldType", QCSData::Integer);
    else if (type == "double")
        data->setProperty("fieldType", QCSData::Double);
    else if (type == "enum") {
        data->setProperty("fieldType", QCSData::Enum);
        data->setProperty("stateStrings", _objectsMap[name]["enums"].toArray().toVariantList());
    }
}

void QCSDataEngineLocal::create(QCSData *data)
{
    // source is of form "loc://<name>[.{"field":"value"}]
    QString source = data->property("source").toString();

    if (!source.startsWith("loc://")) {
        qCritical() << "Not supported source" << source;
        return;
    }

    // find out the root name and json object if present
    QStringList parts = source.split('.', QString::SkipEmptyParts);
    if (parts.length() == 1) {
        QString rootName = parts[0];
        data->setProperty("rootName", rootName);
        // group CSData instances of the same source for convenience
        _dataMap[rootName].append(data);
        // if this source is already configured, populate CSData instance
        if (_objectsMap.contains(rootName)) {
            notifyDataChange(data, rootName);
        }
    } else if (parts.length() == 2) {
        QString rootName = parts[0];
        data->setProperty("rootName", rootName);
        // the supplied json object configures this source if it does not yet exits
        QJsonDocument json = QJsonDocument::fromJson(parts[1].toLocal8Bit());
        if (!json.isObject())
            return;
        if (!_objectsMap.contains(rootName)) {
            _objectsMap.insert(rootName, json.object());
        }
        // group CSData instances of the same source for convenience
        _dataMap[rootName].append(data);
        // populate CSData instances referring this source
        for(int i=0; i<_dataMap[rootName].length(); i++) {
            QCSData *d = _dataMap[rootName][i];
            notifyDataChange(d, rootName);
        }
    } else {
        qCritical() << "Not supported source" << source;
        return;
    }
    // update common constant properties
    data->setProperty("count", 1);
    data->setProperty("accessRight", int(QCSData::ReadAccess | QCSData::WriteAccess));

    data->setProperty("host", "local data");
    data->setProperty("connected", true);
    QCSDataAlarm *alarm= qvariant_cast<QCSDataAlarm*>(data->property("alarm"));
    if (alarm)
        alarm->setProperty("severity", QCSDataAlarm::NoAlarm);

    _data->append(data);
    emit allDataChanged();
}

void QCSDataEngineLocal::close(QCSData *data)
{
    // remove CSData references
    QString rootName = data->property("rootName").toString();
    _dataMap[rootName].removeAll(data);
    if (_dataMap[rootName].length() == 0) {
        _dataMap.remove(rootName);
        _objectsMap.remove(rootName);
    }
    _data->remove(data);
    emit allDataChanged();
}

void QCSDataEngineLocal::setValue(QCSData *data, const QVariant value)
{
    if (!_data->contains(data))
        return;

    // update the value to the json object
    QString rootName = data->property("rootName").toString();
    _objectsMap[rootName]["value"] = QJsonValue::fromVariant(value);

    // notify the change to all data
    for(int i=0; i<_dataMap[rootName].length(); i++) {
        QCSData *d = _dataMap[rootName][i];
        if (d != data)
        notifyDataChange(d, rootName);
    }
}

ObjectModel *QCSDataEngineLocal::allData()
{
    return _data;
}
