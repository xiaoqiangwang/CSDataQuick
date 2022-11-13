#ifndef CSDATAENGINEWS
#define CSDATAENGINEWS

#include "csdataengine.h"

class QWebSocket;

class QCSDataEngineWS : public QCSDataEngine
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ch.psi.sls.CSDataEngineInterface")
    Q_INTERFACES(QCSDataEngine)

public:
    QCSDataEngineWS(QObject *parent=Q_NULLPTR);
    virtual ~QCSDataEngineWS();

    virtual QString name();
    virtual QString description();

    virtual void create(QCSData *data);
    virtual void close(QCSData *data);
    virtual void setValue(QCSData *data, const QVariant value);
    virtual ObjectModel *allData();

protected:
    void ws_message(const QByteArray &message);
    void ws_connected();
    void ws_disconnected();

private:
    ObjectModel *_data;
    QVector<QCSData*> _pending;
    QMap<double, QCSData *> _table;
    QWebSocket *_ws;
    bool _connected;
    double _cid;
};

#endif // CSDATAENGINEWS

