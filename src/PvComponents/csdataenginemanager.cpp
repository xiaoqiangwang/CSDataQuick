#include "csdataenginemanager.h"
#include "csdataenginesim.h"
#include "csdataengineca.h"

CSDataEngineManager *CSDataEngineManager::_manager = Q_NULLPTR;

CSDataEngineManager::CSDataEngineManager(QObject *parent)
    : QObject(parent)
{
    _engines.append(new CSDataEngineSim(this));
    _engines.append(new CSDataEngineCA(this));
}

CSDataEngineManager::~CSDataEngineManager()
{
    qDeleteAll(_engines);
}

CSDataEngineManager *CSDataEngineManager::instance()
{
    if(!_manager)
        _manager = new CSDataEngineManager();
    return _manager;
}

CSDataEngine *CSDataEngineManager::engineForName(QString name)
{
    foreach (CSDataEngine *engine, _engines) {
        if (engine->name() == name)
            return engine;
    }
    return Q_NULLPTR;
}
