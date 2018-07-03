#include "csdata.h"

#include <QCoreApplication>
#include <QtDebug>
#include <QTimer>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    if (argc != 2) {
        qCritical() << "test_qcsdata <data_source>";
        return -1;
    }

    QCSData *data = new QCSData();
    QCSDataAlarm *alarm = data->property("alarm").value<QCSDataAlarm*>();
    QCSDataRange *range = data->property("range").value<QCSDataRange*>();
    data->setSource(argv[1]);

    // enter event loop for 2000 ms
    QTimer::singleShot(2000, &app, SLOT(quit()));
    app.exec();

    // dump data
    qDebug().noquote()
        << data->source() << "\n"
        << "State:     " << (data->connected() ? "Connected" : "Not connected") << "\n"
        << "Host:      " << data->host() << "\n"
        << "Type:      " << data->fieldType() << "\n"
        << "Count:     " << data->count() << "\n"
        << "Value:     " << data->value().toDouble() << "\n"
        << "Alarm:\n"
        << "    Severity: " << alarm->property("severity").toString() << "\n"
        << "    Status:   " << alarm->property("status").toInt() << "\n"
        << "    Message:  " << alarm->property("message").toString() << "\n"
        << "Timestamp: " << data->timeStamp().toString() << "\n"
        << "Range:      [" << range->property("lower").toDouble() << "," << range->property("upper").toDouble() << "]\n"
        << "Units:     " << data->units() << "\n"
        << "Enums:     " << data->stateStrings();

}
