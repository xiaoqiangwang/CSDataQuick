#ifndef CSDATAENGINEMANAGER
#define CSDATAENGINEMANAGER

#include <QObject>
#include <QMap>
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
    QCSDataEngineManager(QObject *parent=Q_NULLPTR);

    static QCSDataEngineManager *_manager;

    QMap<QString, QCSDataEngine*> _engines;

    QCSDataEngine *_defaultEngine;
};

#endif // CSDATAENGINEMANAGER

