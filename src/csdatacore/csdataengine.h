#ifndef CSDATAENGINE_H
#define CSDATAENGINE_H

#include <QString>
#include <QVariant>

class QCSData;

class QCSDataEngine
{
public:
    virtual ~QCSDataEngine() {}

    virtual QString name() = 0;
    virtual void create(QCSData* data) = 0;
    virtual void close(QCSData* data) = 0;
    virtual void setValue(QCSData* data, const QVariant value) = 0;
};

Q_DECLARE_INTERFACE(QCSDataEngine, "ch.psi.sls.QCSDataEngine/1.0")

#endif // CSDATAENGINE_H
