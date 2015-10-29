#ifndef CSDATAENGINECA_H
#define CSDATAENGINECA_H

#include "csdataengine.h"
struct ca_client_context;

class ObjectModel;

class QCSDataEngineCA : public QCSDataEngine
{
    Q_OBJECT
    Q_INTERFACES(QCSDataEngine)

public:
    QCSDataEngineCA(QObject *parent);
    virtual ~QCSDataEngineCA();

    virtual QString name();
    virtual QString description();

    virtual void create(QCSData *data);
    virtual void close(QCSData *data);
    virtual void setValue(QCSData *data, const QVariant value);
    virtual ObjectModel* allData();

signals:

public slots:

private:
    ObjectModel *_data;

    struct ca_client_context *_cac; // global channel access client context
};

#endif // CSDATAENGINECA_H
