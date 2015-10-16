#include "csdataenginemanager.h"
#include "csdataenginesim.h"
#include "csdataengineca.h"
#include "csdata.h"

#include <QtDebug>

QCSDataEngineManager *QCSDataEngineManager::_manager = Q_NULLPTR;

QCSDataEngineManager::QCSDataEngineManager(QObject *parent)
    : QObject(parent)
{
    _engines.append(new QCSDataEngineCA(this));
    _engines.append(new QCSDataEngineSim(this));
}

QCSDataEngineManager::~QCSDataEngineManager()
{
    qDeleteAll(_engines);
}

QCSDataEngineManager *QCSDataEngineManager::instance()
{
    if(!_manager)
        _manager = new QCSDataEngineManager();
    return _manager;
}

QCSDataEngine *QCSDataEngineManager::defaultEngine() const
{
    return _engines.at(0);
}

QCSDataEngine *QCSDataEngineManager::engineForName(QString source) const
{
    QCSDataSourceName dataSourceName(source);
    if (dataSourceName.scheme().isEmpty())
        return defaultEngine();

    foreach (QCSDataEngine *engine, _engines) {
        if (engine->name() == dataSourceName.scheme())
            return engine;
    }
    return Q_NULLPTR;
}

QList<QObject*> QCSDataEngineManager::engines() const
{
    QList<QObject *> objects;
    foreach (QCSDataEngine *engine, _engines) {
        objects.append(qobject_cast<QObject*>(engine));
    }
    return objects;
}
