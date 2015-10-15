#include "csdataenginemanager.h"
#include "csdataenginesim.h"
#include "csdataengineca.h"
#include "csdata.h"

#include <QtDebug>

QCSDataEngineManager *QCSDataEngineManager::_manager = Q_NULLPTR;

QCSDataEngineManager::QCSDataEngineManager(QObject *parent)
    : QObject(parent)
{
    _engines.append(new QCSDataEngineSim(this));
    _engines.append(new QCSDataEngineCA(this));
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

QCSDataEngine *QCSDataEngineManager::defaultEngine()
{
    return _engines.at(1);
}

QCSDataEngine *QCSDataEngineManager::engineForName(QString source)
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
