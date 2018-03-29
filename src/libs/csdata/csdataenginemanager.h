#ifndef CSDATAENGINEMANAGER
#define CSDATAENGINEMANAGER

#include <QObject>
#include "csdata_global.h"

#define engineManager QCSDataEngineManager::instance()

class QCSDataEngine;

class CSDATA_EXPORT QCSDataEngineManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> engines READ engines CONSTANT)

public:
    static QCSDataEngineManager *instance();
    ~QCSDataEngineManager();
    QCSDataEngine *engineForName(QString source) const;
    QCSDataEngine *defaultEngine() const;
    QList<QObject*> engines() const;

private:
    QCSDataEngineManager(QObject *parent=0);

    static QCSDataEngineManager *_manager;

    QList<QCSDataEngine*> _engines;
};

#endif // CSDATAENGINEMANAGER

