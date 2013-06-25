#include "plugin.h"
#include "pvobject.h"
#include "adimageprovider.h"

#include <qqml.h>

void PvComponentsPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<PvObject>(uri, 1, 0, "PvObject");
}

void PvComponentsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    engine->addImageProvider("areaDetector", new ADImageProvider);
}

