#include "utils.h"
#include "enums.h"
#include "csdata.h"

#include <QCoreApplication>
#include <QTimer.h>
#include <QtDebug>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCSData *wavec = new QCSData();
    wavec->setSource("wavec");

    QCSData *ao = new QCSData();
    ao->setSource("ao");

    QCSData *bo = new QCSData();
    bo->setSource("bo");

    // enter event loop for 2000 ms
    QTimer::singleShot(2000, &app, SLOT(quit()));
    app.exec();

    // format the value
    QCSUtils utils;
    qDebug().noquote() << "wavec: ";
    qDebug().noquote() << "    String:  "  << utils.toString(wavec, TextFormat::String, 5);
    qDebug().noquote() << "    Decimal: " << utils.toString(wavec, TextFormat::Decimal, 5);
    qDebug().noquote() << "ao:    " << utils.toString(ao, TextFormat::String, 5);
    qDebug().noquote() << "bo:    " << utils.toString(bo, TextFormat::String, 5);
}
