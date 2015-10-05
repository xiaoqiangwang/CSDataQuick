#ifndef CSDATAENGINECA_H
#define CSDATAENGINECA_H

#include "csdataengine.h"

struct ca_client_context;

class CSDataEngineCA : public QObject, public CSDataEngine
{
    Q_OBJECT
    Q_INTERFACES(CSDataEngine)

public:
    CSDataEngineCA(QObject *parent);
    virtual ~CSDataEngineCA();

    virtual QString name();

    virtual void create(CSData *data);
    virtual void close(CSData *data);
    virtual void setValue(CSData *data, const QVariant value);

signals:

public slots:

private:
    QList<CSData*> _data;

    struct ca_client_context *_cac; // global channel access client context
};

#endif // CSDATAENGINECA_H
