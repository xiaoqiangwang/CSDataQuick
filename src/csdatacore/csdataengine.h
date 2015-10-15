#ifndef CSDATAENGINE_H
#define CSDATAENGINE_H

#include <QString>
#include <QVariant>
#include <QObject>

class QCSData;

class QCSDataEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> allData READ allData)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)

public:
    QCSDataEngine(QObject *parent);
    virtual ~QCSDataEngine() {}

    virtual QString name() {return "";}
    virtual QString description() {return "";}
    virtual void create(QCSData* data) = 0;
    virtual void close(QCSData* data) = 0;
    virtual void setValue(QCSData* data, const QVariant value) = 0;
    virtual QList<QObject*> allData() {return QList<QObject*>();}
};

Q_DECLARE_INTERFACE(QCSDataEngine, "ch.psi.sls.QCSDataEngine/1.0")

#endif // CSDATAENGINE_H
