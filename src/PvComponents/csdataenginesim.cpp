#include "csdataenginesim.h"
#include "csdata.h"

#include <QtDebug>
#include <QtMath>
#include <QTimerEvent>
#include <QUrl>

CSDataEngineSim::CSDataEngineSim(QObject *parent)
    : QObject(parent)
{
    // data for "sim://wave"
    for(int i=0; i<20; i++)
        wave.append(0);

    _timerId = 0;
}

CSDataEngineSim::~CSDataEngineSim()
{
    killTimer(_timerId);
}

QString CSDataEngineSim::name()
{
    return "sim";
}

void CSDataEngineSim::create(CSData *data)
{
    QUrl source = data->property("source").toUrl();
    if (source.scheme() != name())
        return;

    if (source == QStringLiteral("sim://random") || source == QStringLiteral("sim://sin")) {
        data->setProperty("value", 0);
        data->setProperty("fieldType", CSData::Double);
        data->setProperty("count", 1);
        data->setProperty("accessRight", CSData::ReadAccess);
    } else if (source == QStringLiteral("sim://enum")) {
        data->setProperty("value", 0);
        data->setProperty("fieldType", CSData::Enum);
        data->setProperty("count", 1);
        data->setProperty("accessRight", int(CSData::ReadAccess | CSData::WriteAccess));
        QStringList states;
        states << "OFF" << "ON";
        data->setProperty("stateStrings", states);
    } else if (source == QStringLiteral("sim://wave")) {
        data->setProperty("value", 0);
        data->setProperty("fieldType", CSData::Double);
        data->setProperty("count", 20);
        data->setProperty("accessRight", CSData::ReadAccess);
    } else {
        qCritical() << "Not supported source" << source;
        return;
    }
    data->setProperty("host", "simulator");
    data->setProperty("connected", true);
    CSDataAlarm *alarm= qvariant_cast<CSDataAlarm*>(data->property("alarm"));
    if (alarm)
        alarm->setProperty("severity", CSDataAlarm::NoAlarm);
    _data.append(data);

    if (!_timerId)
        _timerId = startTimer(1000);
}

void CSDataEngineSim::close(CSData *data)
{
    _data.removeAll(data);
    if (_data.length() == 0) {
        killTimer(_timerId);
        _timerId = 0;
    }
}

void CSDataEngineSim::setValue(CSData *data, const QVariant value)
{
    if (!_data.contains(data))
        return;

    if (data->property("source") == "sim://enum")
        data->setProperty("value", value.toInt());
}

void CSDataEngineSim::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != _timerId)
        return;

    static int count = 0;

    count++;
    foreach(CSData *data, _data) {
        if (data->property("source") == "sim://random") {
            data->setProperty("value", qreal(qrand() * 1.0 / RAND_MAX));
            data->setProperty("timeStamp", QDateTime());
        }
        else if (data->property("source") == "sim://sin") {
            data->setProperty("value", qSin(count));
            data->setProperty("timeStamp", QDateTime());
        }
        else if (data->property("source") == "sim://wave") {
            for(int i=0; i<20; i++)
                wave[i] = qSin(i + count);
            data->setProperty("value", QVariant::fromValue(wave));
        }
    }
}
