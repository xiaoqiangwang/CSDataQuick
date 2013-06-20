#include "pvobject_plugin.h"
#include "pvobject.h"

#include <qqml.h>

void PvObjectPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<PvObject>(uri, 1, 0, "PvObject");
}

