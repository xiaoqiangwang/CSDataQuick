#include "csdataenginemanager.h"
#include "csdataenginesim.h"
#include "csdataengineca.h"

#include <QtDebug>
#include <QUrl>

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
    QUrl url(source);
    if (url.scheme().isEmpty())
        return defaultEngine();

    foreach (QCSDataEngine *engine, _engines) {
        if (engine->name() == url.scheme())
            return engine;
    }
    return Q_NULLPTR;
}
