#ifndef CSDATAENGINESIM
#define CSDATAENGINESIM

#include "csdataengine.h"

#include "objectmodel.h"

class QCSDataEngineSim : public QCSDataEngine
{
    Q_OBJECT
    //Q_PLUGIN_METADATA(IID "ch.psi.sls.CSDataEngineInterface")
    Q_INTERFACES(QCSDataEngine)

public:
    QCSDataEngineSim(QObject *parent=0);
    virtual ~QCSDataEngineSim();

    virtual QString name();
    virtual QString description();

    virtual void create(QCSData *data);
    virtual void close(QCSData *data);
    virtual void setValue(QCSData *data, const QVariant value);
    virtual ObjectModel *allData();

protected:
    void timerEvent(QTimerEvent *);

private:
    ObjectModel *_data;

    QVariantList wave;

    int _timerId;
};

#endif // CSDATAENGINESIM

