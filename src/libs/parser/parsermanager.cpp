/*****************************************************************************\
 * Copyright 2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "parsermanager.h"
#include "parser.h"

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
}
#elif defined(Q_OS_WIN32)
BOOL WINAPI DllMain(
        __in  HINSTANCE hinstDLL,
        __in  DWORD fdwReason,
        __in  LPVOID lpvReserved) {

    GetModuleFileNameA(hinstDLL, libraryFilePath, MAX_PATH);
    return TRUE;
}
#endif

QCSParserManager *QCSParserManager::_manager = Q_NULLPTR;

/*!
    \class QCSParserManager
    \inmodule CSDataQuick.Parser
    \brief Singleton object to load/query file parsers.

    The parser manager loads all parser plugins in 
    path ../plugins/parser relative to the library itself.
*/

/*!
    \property QCSParserManager::parsers
    List of display file parsers.
*/

QCSParserManager::QCSParserManager(QObject *parent)
    : QObject(parent)
{
    QDir pluginsDir = QFileInfo(libraryFilePath).dir();
    pluginsDir.cdUp();
    pluginsDir.cd("plugins");
    pluginsDir.cd("csparser");
    foreach(QFileInfo fileInfo, pluginsDir.entryInfoList(QDir::Files)) {
        QPluginLoader loader(fileInfo.absoluteFilePath());
        QCSParser *parser = qobject_cast<QCSParser*>(loader.instance());
        if (parser) {
            _parsers.append(parser);
        }
    }
    foreach(QStaticPlugin plugin, QPluginLoader::staticPlugins()) {
        QCSParser *parser = qobject_cast<QCSParser*>(plugin.instance());
        if (parser) {
            _parsers.append(parser);
        }
    }
}

QCSParserManager::~QCSParserManager()
{
    qDeleteAll(_parsers);
}

/*!
    Returns the singleton instance
 */
QCSParserManager *QCSParserManager::instance()
{
    if(!_manager)
        _manager = new QCSParserManager();
    return _manager;
}

/*!
    Returns the file parser registered for the file \a extension.
  */
QCSParser *QCSParserManager::parserForExtension(QString extension) const
{
    foreach (QCSParser *parser, _parsers) {
        if (parser->extension() == extension)
            return parser;
    }
    return Q_NULLPTR;
}

/*!
    Returns the list of supported file extensions.
  */
QStringList QCSParserManager::supportedExtensions() const
{
    QStringList extensions;
    foreach (QCSParser *parser, _parsers) {
        extensions.append(parser->extension());
    }
    return extensions;
}
