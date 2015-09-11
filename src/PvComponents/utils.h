#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QVariant>


class QCSUtils : public QObject
{
    Q_OBJECT
public:
    explicit QCSUtils(QObject *parent = 0);

    Q_INVOKABLE double calculate(QString expr, QVariantList input);
    Q_INVOKABLE bool execute(QString program);
    Q_INVOKABLE QString format(QString format, double number);
    Q_INVOKABLE QString convert(int format, QVariant value, int precision);
    Q_INVOKABLE double parse(int format, QString textValue);

signals:

public slots:

};
#endif // UTILS_H