#include "csdataenginesim.h"
#include "csdata.h"
#include "objectmodel.h"

#include <QtDebug>
#include <QtMath>
#include <QTimerEvent>
#include <QUrl>

/*!
 * \class QCSDataEngineSim
 * \inmodule CSDataQuick.Data
 * \ingroup csdataquick.data.plugins
 * \brief Simulation data produced by math functions.
 *
 * This data engine source has a \e sim:// scheme, followed by the data name.
 * The following data names are available:
 * \table
 * \header
 *     \li Data name
 *     \li Data type
 *     \li Data Count
 *     \li Update
 *     \li Description
 * \row
 *     \li random
 *     \li double
 *     \li 1
 *     \li 1 Hz
 *     \li radom number between 0.0 and 1.0.
 * \row
 *     \li sin
 *     \li double
 *     \li 1
 *     \li 1 Hz
 *     \li sinous function of an interal integer counter.
 * \row
 *     \li wave
 *     \li double
 *     \li 20
 *     \li 1 Hz
 *     \li sinous of an integer array, in which the first element is the internal counter.
 * \row
 *     \li enum
 *     \li enum
 *     \li 1
 *     \li Passive
 *     \li Enum value with two states, OFF and ON.
 * \endtable
 */
QCSDataEngineSim::QCSDataEngineSim(QObject *parent)
    : QCSDataEngine(parent)
{
    _data = new ObjectModel(this);

    QByteArrayList roles;
    roles << "source" << "connected";
    _data->setup<QCSData>(roles);

    // data for "sim://wave"
    for(int i=0; i<20; i++)
        wave.append(0);

    _timerId = 0;
}

QCSDataEngineSim::~QCSDataEngineSim()
{
    killTimer(_timerId);
}

/*!
 * \reimp
 * \brief Returns the source scheme \e sim.
 */
QString QCSDataEngineSim::name()
{
    return "sim";
}

/*!
 * \reimp
 */
QString QCSDataEngineSim::description()
{
    return "Simulation Engine";
}

/*!
 * \reimp
 */
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
    _data->append(data);
    emit allDataChanged();

    if (!_timerId)
        _timerId = startTimer(1000);
}

/*!
 * \reimp
 */
void QCSDataEngineSim::close(QCSData *data)
{
    _data->remove(data);
    emit allDataChanged();

    if (_data->size() == 0) {
        killTimer(_timerId);
        _timerId = 0;
    }
}

/*!
 * \reimp
 */
void QCSDataEngineSim::setValue(QCSData *data, const QVariant value)
{
    if (!_data->contains(data))
        return;

    if (data->property("source") == "sim://enum")
        data->updateValue(value.toInt());
}

/*!
 * \internal
 */
void QCSDataEngineSim::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != _timerId)
        return;

    static int count = 0;

    count++;
    for(int i=0; i<_data->size(); i++) {
        QCSData *data = qobject_cast<QCSData*>(_data->at(i));
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

/*!
 * \reimp
 */
ObjectModel *QCSDataEngineSim::allData()
{
    return _data;
}
