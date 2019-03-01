#ifndef CSDATAENGINELOCAL
#define CSDATAENGINELOCAL

#include "csdataengine.h"
#include <QMap>
#include <QJsonObject>

class QCSDataEngineLocal : public QCSDataEngine
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.psi.sls.CSDataEngineInterface")
    Q_INTERFACES(QCSDataEngine)

public:
    QCSDataEngineLocal(QObject *parent=Q_NULLPTR);
    virtual ~QCSDataEngineLocal();

    virtual QString name();
    virtual QString description();

    virtual void create(QCSData *data);
    virtual void close(QCSData *data);
    virtual void setValue(QCSData *data, const QVariant value);
    virtual ObjectModel *allData();

protected:
    void initializeData(QCSData *data, QString name);

private:
    ObjectModel *_data;
    QMap<QString, QJsonObject> _objectsMap;
    QMap<QString, QList<QCSData*>> _dataMap;
};

#endif // CSDATAENGINELOCAL
