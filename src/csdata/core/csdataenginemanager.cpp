#include "csdataenginemanager.h"
#include "csdataenginesim.h"
#include "csdata.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QCoreApplication>

#include <QtDebug>

#if !defined(MAX_PATH) && defined(PATH_MAX)
#define MAX_PATH PATH_MAX
#endif

static char libraryFilePath[MAX_PATH];

#if defined(Q_OS_LINUX) || defined(Q_OS_OSX)
#include <dlfcn.h>
__attribute__((constructor))
static void init(void) {
    Dl_info dl_info;
    dladdr((void*)init, &dl_info);
    strncpy(libraryFilePath, dl_info.dli_fname, MAX_PATH-1);
    libraryFilePath[MAX_PATH-1] = '\0';
}
#elif defined(Q_OS_WIN32)
BOOL WINAPI DllMain(
        __in  HINSTANCE hinstDLL,
        __in  DWORD fdwReason,
        __in  LPVOID lpvReserved) {

    GetModuleFileName(hinstDLL, libraryFilePath, MAX_PATH);
}
#endif

QCSDataEngineManager *QCSDataEngineManager::_manager = Q_NULLPTR;

QCSDataEngineManager::QCSDataEngineManager(QObject *parent)
    : QObject(parent)
{
    _engines.append(new QCSDataEngineSim(this));


    QDir pluginsDir = QFileInfo(libraryFilePath).dir();
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
