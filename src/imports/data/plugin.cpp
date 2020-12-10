/*****************************************************************************\
 * Copyright 2012-2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "plugin.h"
#include "csdata.h"
#include "csdataengine.h"
#include "csdataenginemanager.h"
#include "objectmodel.h"
#include "sortfilterproxymodel.h"

#include <qqml.h>

static QObject *enginemanager_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return engineManager;
}

void CSDataPlugin::registerTypes(const char *uri)
{
    // @uri CSDataQuick.Data
    qmlRegisterType<QCSData>(uri, 1, 0, "CSData");
    qmlRegisterType<QCSDataRange>(uri, 1, 0, "CSDataRange");
    qmlRegisterType<QCSDataAlarm>(uri, 1, 0, "CSDataAlarm");
    qmlRegisterType<SortFilterProxyModel>(uri, 1, 0, "SortFilterProxyModel");
    qmlRegisterUncreatableType<ObjectModel>(uri, 1, 0, "ObjectModel", "QObject List Model");

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    qmlRegisterInterface<QCSDataEngine>("CSDataEngine", 1);
#else
    qmlRegisterInterface<QCSDataEngine>("CSDataEngine");
#endif

    qmlRegisterSingletonType<QCSDataEngineManager>(uri, 1, 0, "DataEngineManager", enginemanager_provider);

    qRegisterMetaType<QCSData::AccessFlags>("CSData::AccessFlags");
    qRegisterMetaType<QCSData::FieldType>("CSData::FieldType");
    qRegisterMetaType<QCSDataAlarm::Severity>("CSDataAlarm::Severity");
}
