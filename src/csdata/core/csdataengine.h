#ifndef CSDATAENGINE_H
#define CSDATAENGINE_H

#include <QString>
#include <QVariant>
#include <QObject>

class QCSData;
class ObjectModel;

class QCSDataEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ObjectModel* allData READ allData NOTIFY allDataChanged)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)

public:
    QCSDataEngine(QObject *parent);
    virtual ~QCSDataEngine() {}

    virtual QString name() = 0;
    virtual QString description() = 0;
    virtual void create(QCSData* data) = 0;
    virtual void close(QCSData* data) = 0;
    virtual void setValue(QCSData* data, const QVariant value) = 0;
    virtual ObjectModel * allData() = 0;

signals:
    void allDataChanged();
};

Q_DECLARE_INTERFACE(QCSDataEngine, "ch.psi.sls.QCSDataEngine/1.0")

#endif // CSDATAENGINE_H
