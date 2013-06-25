#include "adimageprovider_plugin.h"
#include "adimageprovider.h"

void ADImageProviderPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);
}

void ADImageProviderPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    engine->addImageProvider("areaDetector", new ADImageProvider);
}
