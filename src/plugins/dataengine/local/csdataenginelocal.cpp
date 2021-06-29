/*****************************************************************************\
 * Copyright 2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "csdataenginelocal.h"
#include "csdata.h"
#include "objectmodel.h"

#include <QtDebug>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>

#include <iostream>

/*!
 * \class QCSDataEngineLocal
 * \inmodule CSDataQuick.Data
 * \ingroup csdataquick.data.plugins
 * \brief User configurable data.
 *
 * This data engine source has the form \e loc://<name>[.{"field":"value"}]. The trailing part after \e . is a JSON string.
 * It could be used to configure the data and has the following fields:
 * \table
 * \header
 *     \li Field
 *     \li Description
 * \row
 *     \li type
 *     \li Data type. Any of \e enum, \e string, \e int and \e double.
 * \row
 *     \li value
 *     \li Initial value.
 * \row
 *     \li enums
 *     \li Array of state strings for \e enum type.
 * \row
 *     \li precision
 *     \li Precision of a \e double typed value.
 * \endtable
 *
 * The data is not signaled as connected until it gets configured. If already configured, the second configurtion takes no effect.
 *
 * \code
 *     loc://real.{"type": "double", "value": 3.14, "precision": 2}
 *     loc://integer.{"type": "int", "value": 3}
 *     loc://string.{"type": "string", "value": "string message"}
 *     loc://enum.{"type": "enum", "enums": ["off", "on"], "value": 1}
 * \endcode
 */
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

/*!
 * \reimp
 * \brief Returns the source scheme \e loc.
 */
QString QCSDataEngineLocal::name()
{
    return "loc";
}

/*!
 * \reimp
 */
QString QCSDataEngineLocal::description()
{
    return "Local Data Engine";
}

/*!
 * \internal
 */
void QCSDataEngineLocal::initializeData(QCSData *data, QString name)
{
    data->updateValue(_objectsMap[name]["value"].toVariant());
    // update common constant properties
    QString type = _objectsMap[name]["type"].toString();
    if (type == "string")
        data->setProperty("fieldType", QCSData::String);
    else if (type == "int")
        data->setProperty("fieldType", QCSData::Integer);
    else if (type == "double") {
        data->setProperty("fieldType", QCSData::Double);
        data->setProperty("precision", _objectsMap[name]["precision"].toInt());
    }
    else if (type == "enum") {
        data->setProperty("fieldType", QCSData::Enum);
        data->setProperty("stateStrings", _objectsMap[name]["enums"].toArray().toVariantList());
    }
    data->setProperty("count", 1);
    data->setProperty("accessRight", int(QCSData::ReadAccess | QCSData::WriteAccess));

    data->setProperty("host", "local data");
    data->setProperty("connected", true);
    QCSDataAlarm *alarm= qvariant_cast<QCSDataAlarm*>(data->property("alarm"));
    if (alarm)
        alarm->setProperty("severity", QCSDataAlarm::NoAlarm);
}

/*!
 * \reimp
 */
void QCSDataEngineLocal::create(QCSData *data)
{
    // source is of form "loc://<name>[.{"field":"value"}]
    QString source = data->property("source").toString();

    if (!source.startsWith("loc://")) {
        qCritical() << "Not supported source" << source;
        return;
    }

    // find out the root name and json object if present
    int npos = source.indexOf('.');
    QString rootName = source.left(npos);
    if (npos == -1) {
        data->setProperty("rootName", rootName);
        // group CSData instances of the same source for convenience
        _dataMap[rootName].append(data);
        // if this source is already configured, populate CSData instance
        if (_objectsMap.contains(rootName)) {
            initializeData(data, rootName);
        }
    } else {
        data->setProperty("rootName", rootName);
        // the supplied json object configures this source if it does not yet exits
        QJsonDocument json = QJsonDocument::fromJson(source.mid(npos+1).toLocal8Bit());
        if (!json.isObject()) {
            qCritical() << "Invalid JSON configuration" << source;
            return;
        }
        if (!_objectsMap.contains(rootName)) {
            _objectsMap.insert(rootName, json.object());
        }
        // group CSData instances of the same source for convenience
        _dataMap[rootName].append(data);
        // populate CSData instances referring this source
        for(int i=0; i<_dataMap[rootName].length(); i++) {
            QCSData *d = _dataMap[rootName][i];
            initializeData(d, rootName);
        }
    }
    _data->append(data);
    emit allDataChanged();
}

/*!
 * \reimp
 */
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

/*!
 * \reimp
 */
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
        d->updateValue(_objectsMap[rootName]["value"].toVariant());
    }
}

/*!
 * \reimp
 */
ObjectModel *QCSDataEngineLocal::allData()
{
    return _data;
}
