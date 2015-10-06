#ifndef CSDATAENGINECA_H
#define CSDATAENGINECA_H

#include "csdataengine.h"

struct ca_client_context;

class QCSDataEngineCA : public QObject, public QCSDataEngine
{
    Q_OBJECT
    Q_INTERFACES(QCSDataEngine)

public:
    QCSDataEngineCA(QObject *parent);
    virtual ~QCSDataEngineCA();

    virtual QString name();

    virtual void create(QCSData *data);
    virtual void close(QCSData *data);
    virtual void setValue(QCSData *data, const QVariant value);

signals:

public slots:

private:
    QList<QCSData*> _data;

    struct ca_client_context *_cac; // global channel access client context
};

#endif // CSDATAENGINECA_H
