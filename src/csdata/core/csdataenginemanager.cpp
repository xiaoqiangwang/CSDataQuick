#include "csdataenginemanager.h"
#include "csdataenginesim.h"
#include "csdata.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QCoreApplication>

#include <QtDebug>

QCSDataEngineManager *QCSDataEngineManager::_manager = Q_NULLPTR;

QCSDataEngineManager::QCSDataEngineManager(QObject *parent)
    : QObject(parent)
{
    _engines.append(new QCSDataEngineSim(this));

    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cdUp();
    pluginsDir.cd("plugins");
    foreach(QFileInfo fileInfo, pluginsDir.entryInfoList(QDir::Files)) {
        QPluginLoader loader(fileInfo.absoluteFilePath());
        QCSDataEngine *engine = qobject_cast<QCSDataEngine*>(loader.instance());
        if (engine) {
            _engines.append(engine);
            qDebug() << "Loaded " << engine->description();
        }
    }
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
    foreach (QCSDataEngine *engine, _engines) {
        if (engine->name() == "ca")
            return engine;
    }
    return Q_NULLPTR;
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
