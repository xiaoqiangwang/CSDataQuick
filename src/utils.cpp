#include "utils.h"
#include <postfix.h>
#include <QVector>
#include <QVariant>
#include <QProcess>
#include <QtDebug>

Utils::Utils(QObject *parent) :
    QObject(parent)
{
}

QString Utils::format(QString format, double number)
{
    return QString("").sprintf(format.toLatin1(), number);
}

double Utils::calculate(QString expr, QVariantList input)
{
    double result = 0.0;
    short error;
    char infix[INFIX_TO_POSTFIX_SIZE(80)];
    
    QVector<double> d;
    foreach(QVariant v, input)
        d.push_back(v.toDouble());
    
    postfix(expr.toLatin1().constData(), infix, &error);
    
    calcPerform(d.data(), &result, infix);

    return result;
}

bool Utils::execute(QString program)
{
    return QProcess::startDetached(program);
}
