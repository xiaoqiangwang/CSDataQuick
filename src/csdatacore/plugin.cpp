/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "plugin.h"
#include "csdata.h"
#include "csdataengine.h"
#include "csdataenginemanager.h"

#include <qqml.h>

void CSDataPlugin::registerTypes(const char *uri)
{
    // @uri CSDataQuick.Data
    qmlRegisterType<QCSData>(uri, 1, 0, "CSData");
    qmlRegisterType<QCSDataRange>(uri, 1, 0, "CSDataRange");
    qmlRegisterType<QCSDataAlarm>(uri, 1, 0, "CSDataAlarm");

    qmlRegisterInterface<QCSDataEngine>("CSDataEngine");

    qRegisterMetaType<QCSData::AccessFlags>("CSData::AccessFlags");
    qRegisterMetaType<QCSData::FieldType>("CSData::FieldType");
    qRegisterMetaType<QCSDataAlarm::Severity>("CSDataAlarm::Severity");
}
