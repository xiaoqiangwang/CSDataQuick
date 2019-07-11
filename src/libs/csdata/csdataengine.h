#ifndef CSDATAENGINE_H
#define CSDATAENGINE_H

#include <QString>
#include <QVariant>

class QCSData;
class ObjectModel;

class QCSDataEngine
{
public:
    virtual ~QCSDataEngine() = default;

    virtual QString name() = 0;
    virtual QString description() = 0;
    virtual void create(QCSData* data) = 0;
    virtual void close(QCSData* data) = 0;
    virtual void setValue(QCSData* data, const QVariant value) = 0;
    virtual ObjectModel * allData() = 0;

protected:
    virtual void allDataChanged() = 0;
};

Q_DECLARE_INTERFACE(QCSDataEngine, "ch.psi.sls.QCSDataEngine/1.0")

#endif // CSDATAENGINE_H
