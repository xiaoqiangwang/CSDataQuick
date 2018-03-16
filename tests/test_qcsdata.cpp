#include "csdata.h"

#include <QCoreApplication>
#include <QThread>
#include <QtDebug>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCSData *data = new QCSData();
    QCSDataAlarm *alarm = data->property("alarm").value<QCSDataAlarm*>();
    QCSDataRange *range = data->property("range").value<QCSDataRange*>();

    data->setSource("bo");
    // wait for connection
    while (!data->connected() || data->timeStamp().toMSecsSinceEpoch() == 0)
        app.processEvents(QEventLoop::AllEvents, 100);

    qDebug() << data->source() << "\n"
             << "Host:      " << data->host() << "\n"
             << "Type:      " << data->fieldType() << "\n"
             << "Value:     " << data->value().toDouble() << "\n"
             << "Alarm:     " << alarm->property("severity") << alarm->property("status") <<  alarm->property("message")<< "\n"
             << "Timestamp: " << data->timeStamp().toString() << "\n"
             << "Range:     " << range->property("lower").toDouble() << ", " << range->property("upper").toDouble();

    data->setSource("");
    data->deleteLater();
}
