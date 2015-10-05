#ifndef CSDATAENGINE_H
#define CSDATAENGINE_H

#include <QString>
#include <QVariant>

class CSData;

class CSDataEngine
{
public:
    virtual ~CSDataEngine() {}

    virtual QString name() = 0;
    virtual void create(CSData* data) = 0;
    virtual void close(CSData* data) = 0;
    virtual void setValue(CSData* data, const QVariant value) = 0;
};

Q_DECLARE_INTERFACE(CSDataEngine, "ch.psi.sls.CSDataEngine/1.0")

#endif // CSDATAENGINE_H
