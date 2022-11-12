/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "csdataenginemanager.h"
#include "csdataengine.h"
#include "csdata.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QCoreApplication>

#include <QtDebug>

#ifdef Q_OS_WIN32
#include <Windows.h>
#if !defined(MAX_PATH) && defined(PATH_MAX)
#define MAX_PATH PATH_MAX
#endif
#endif

#include <limits.h>
#if defined(_POSIX_PATH_MAX) && !defined(MAX_PATH)
#define MAX_PATH _POSIX_PATH_MAX
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

    qRegisterMetaType<QCSDataAlarm::Severity>();
    qRegisterMetaType<QCSData::FieldType>();
}
#elif defined(Q_OS_WIN32)
BOOL WINAPI DllMain(
        __in  HINSTANCE hinstDLL,
        __in  DWORD fdwReason,
        __in  LPVOID lpvReserved) {

    GetModuleFileNameA(hinstDLL, libraryFilePath, MAX_PATH);

    qRegisterMetaType<QCSDataAlarm::Severity>();
    qRegisterMetaType<QCSData::FieldType>();
    return TRUE;
}
#endif

QCSDataEngineManager *QCSDataEngineManager::_manager = Q_NULLPTR;

/*!
    \class QCSDataEngineManager
    \inmodule CSDataQuick.Data
    \brief Singleton object to load/query data engines.


    The engine manager loads all data engine plugins in 
    path ../plugins/csdataengine relative to the library itself.
*/

QCSDataEngineManager::QCSDataEngineManager(QObject *parent)
    : QObject(parent),
      _defaultEngine(Q_NULLPTR)
{
    QStringList paths;
    /* relative plugin path ../plugins */
    QDir dir = QFileInfo(libraryFilePath).dir();
    if (dir.cd("../plugins"))
        paths.append(dir.absolutePath());
    /* QT_PLUGIN_PATH */
    paths.append(QCoreApplication::libraryPaths());
    foreach(QString path, paths) {
        QDir pluginsDir(path);
        if (pluginsDir.cd("csdataengine")) {
            foreach(QFileInfo fileInfo, pluginsDir.entryInfoList(QDir::Files)) {
                QPluginLoader loader(fileInfo.absoluteFilePath());
                QCSDataEngine *engine = qobject_cast<QCSDataEngine*>(loader.instance());
                if (engine) {
                    _engines[engine->name()] = engine;
                    qDebug() << "Loaded " << engine->description();
                }
            }
            /* finish searching if found */
            break;
        }
    }
    foreach(QStaticPlugin plugin, QPluginLoader::staticPlugins()) {
        QCSDataEngine *engine = qobject_cast<QCSDataEngine*>(plugin.instance());
        if (engine) {
            _engines[engine->name()] = engine;
            qDebug() << "Added " << engine->description();
        }
    }

    /* default engine */
    _defaultEngine = _engines.value(qgetenv("CSDQ_DEFAULT_DATAENGINE"), Q_NULLPTR);
    if (!_defaultEngine) {
        qWarning() << "Data engine " << qgetenv("CSDQ_DEFAULT_DATAENGINE") << "(as specified by CSDQ_DEFAULT_DATAENGINE) is not available.";
        /* search a list of known engines */
        foreach (QString name, QStringList({"ca", "ws", "loc", "sim"})) {
            _defaultEngine = _engines.value(name, Q_NULLPTR);
            if (_defaultEngine) {
                qWarning() << "Fallback to data engine "<< _defaultEngine->name();
                break;
            }
        }
    }

    if (!_defaultEngine) {
        qWarning() << "No default data engine found.";
    }
}

QCSDataEngineManager::~QCSDataEngineManager()
{
    qDeleteAll(_engines);
    _engines.clear();
}

/*!
    Returns the singleton instance
 */
QCSDataEngineManager *QCSDataEngineManager::instance()
{
    if(!_manager)
        _manager = new QCSDataEngineManager();
    return _manager;
}

/*! \internal */
QCSDataEngine *QCSDataEngineManager::defaultEngine() const
{
    return _defaultEngine;
}

/*!
    Returns the data engine registered for the scheme of \a source. If no scheme specified,
    the default engine is returned, i.e. "ca".
  */
QCSDataEngine *QCSDataEngineManager::engineForName(QString source) const
{
    QCSDataSourceName dataSourceName(source);
    if (dataSourceName.scheme().isEmpty())
        return _defaultEngine;

    return _engines.value(dataSourceName.scheme(), _defaultEngine);
}

/*!
    \property QCSDataEngineManager::engines
    List of all available data engines.
*/
QList<QObject*> QCSDataEngineManager::engines() const
{
    QList<QObject *> objects;
    foreach (QCSDataEngine *engine, _engines.values()) {
        objects.append(qobject_cast<QObject*>(engine));
    }
    return objects;
}
