#include "pvobject_plugin.h"
#include "pvobject.h"

#include <qqml.h>

void PvObjectPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterType<PvObject>(uri, 1, 0, "PvObject");
}

