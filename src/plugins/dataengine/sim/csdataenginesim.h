#ifndef CSDATAENGINESIM
#define CSDATAENGINESIM

#include "csdataengine.h"

class QCSDataEngineSim : public QObject, public QCSDataEngine
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.psi.sls.CSDataEngineInterface")
    Q_INTERFACES(QCSDataEngine)

    Q_PROPERTY(ObjectModel* allData READ allData NOTIFY allDataChanged)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)

public:
    QCSDataEngineSim(QObject *parent=Q_NULLPTR);
    virtual ~QCSDataEngineSim();

    virtual QString name();
    virtual QString description();

    virtual void create(QCSData *data);
    virtual void close(QCSData *data);
    virtual void setValue(QCSData *data, const QVariant value);
    virtual ObjectModel *allData();

signals:
    void allDataChanged();

protected:
    void timerEvent(QTimerEvent *);

private:
    ObjectModel *_data;

    QVariantList wave;

    int _timerId;
};

#endif // CSDATAENGINESIM

