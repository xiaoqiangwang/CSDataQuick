#ifndef CSDATAENGINESIM
#define CSDATAENGINESIM

#include "csdataengine.h"

class CSDataEngineSim : public QObject, public CSDataEngine
{
    Q_OBJECT
    //Q_PLUGIN_METADATA(IID "ch.psi.sls.CSDataEngineInterface")
    Q_INTERFACES(CSDataEngine)

public:
    CSDataEngineSim(QObject *parent=0);
    virtual ~CSDataEngineSim();

    virtual QString name();

    virtual void create(CSData *data);
    virtual void close(CSData *data);
    virtual void setValue(CSData *data, const QVariant value);

protected:
    void timerEvent(QTimerEvent *);

private:
    QList<CSData*> _data;

    QVariantList wave;

    int _timerId;
};

#endif // CSDATAENGINESIM

