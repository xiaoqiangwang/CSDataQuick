#include "csdataenginesim.h"
#include "csdata.h"

#include <QtDebug>
#include <QtMath>
#include <QTimerEvent>
#include <QUrl>

QCSDataEngineSim::QCSDataEngineSim(QObject *parent)
    : QObject(parent)
{
    // data for "sim://wave"
    for(int i=0; i<20; i++)
        wave.append(0);

    _timerId = 0;
}

QCSDataEngineSim::~QCSDataEngineSim()
{
    killTimer(_timerId);
}

QString QCSDataEngineSim::name()
{
    return "sim";
}

void QCSDataEngineSim::create(QCSData *data)
{
    QString source = data->property("source").toString();

    if (QCSDataSourceName(source).scheme() != name()) {
        qWarning() << "Unsupported source by CSDataEngineSim" << source;
        return;
    }

    if (source == QStringLiteral("sim://random") || source == QStringLiteral("sim://sin")) {
        data->updateValue(0);
        data->setProperty("fieldType", QCSData::Double);
        data->setProperty("count", 1);
        data->setProperty("accessRight", QCSData::ReadAccess);
    } else if (source == QStringLiteral("sim://enum")) {
        data->updateValue(0);
        data->setProperty("fieldType", QCSData::Enum);
        data->setProperty("count", 1);
        data->setProperty("accessRight", int(QCSData::ReadAccess | QCSData::WriteAccess));
        QStringList states;
        states << "OFF" << "ON";
        data->setProperty("stateStrings", states);
    } else if (source == QStringLiteral("sim://wave")) {
        data->updateValue(0);
        data->setProperty("fieldType", QCSData::Double);
        data->setProperty("count", 20);
        data->setProperty("accessRight", QCSData::ReadAccess);
    } else {
        qCritical() << "Not supported source" << source;
        return;
    }
    data->setProperty("host", "simulator");
    data->setProperty("connected", true);
    QCSDataAlarm *alarm= qvariant_cast<QCSDataAlarm*>(data->property("alarm"));
    if (alarm)
        alarm->setProperty("severity", QCSDataAlarm::NoAlarm);
    _data.append(data);

    if (!_timerId)
        _timerId = startTimer(1000);
}

void QCSDataEngineSim::close(QCSData *data)
{
    _data.removeAll(data);
    if (_data.length() == 0) {
        killTimer(_timerId);
        _timerId = 0;
    }
}

void QCSDataEngineSim::setValue(QCSData *data, const QVariant value)
{
    if (!_data.contains(data))
        return;

    if (data->property("source") == "sim://enum")
        data->updateValue(value.toInt());
}

void QCSDataEngineSim::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != _timerId)
        return;

    static int count = 0;

    count++;
    foreach(QCSData *data, _data) {
        if (data->property("source") == "sim://random") {
            data->updateValue(qreal(qrand() * 1.0 / RAND_MAX));
            data->setProperty("timeStamp", QDateTime());
        }
        else if (data->property("source") == "sim://sin") {
            data->updateValue(qSin(count));
            data->setProperty("timeStamp", QDateTime());
        }
        else if (data->property("source") == "sim://wave") {
            for(int i=0; i<20; i++)
                wave[i] = qSin(i + count);
            data->updateValue(QVariant::fromValue(wave));
        }
    }
}
