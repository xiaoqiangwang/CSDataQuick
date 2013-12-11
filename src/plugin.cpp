#include "plugin.h"
#include "pvobject.h"
#include "shapes.h"
#include "utils.h"
#include "adimageprovider.h"

#include <qqml.h>

static QObject *utils_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return new Utils();
}

void PvComponentsPlugin::registerTypes(const char *uri)
{
    // @uri PvComponents
    qmlRegisterType<PvObject>(uri, 1, 0, "PvObject");
    qmlRegisterType<Polyline>(uri, 1, 0, "Polyline");
    qmlRegisterType<Polygon>(uri, 1, 0, "Polygon");
    qmlRegisterType<Arc>(uri, 1, 0, "Arc");
    qmlRegisterType<PaintedRectangle>(uri, 1, 0, "PaintedRectangle");
    qmlRegisterType<Oval>(uri, 1, 0, "Oval");

    qmlRegisterSingletonType<Utils>(uri, 1, 0, "Utils", utils_provider);
    qmlRegisterUncreatableType<TextFormat>(uri, 1, 0, "TextFormat", "TextFomat Enum");
    qmlRegisterUncreatableType<ColorMode>(uri, 1, 0, "ColorMode", "ColorMode Enum");
    qmlRegisterUncreatableType<EdgeStyle>(uri, 1, 0, "EdgeStyle", "EdgeStyle Enum");
    qmlRegisterUncreatableType<FillStyle>(uri, 1, 0, "FillStyle", "FillStyle Enum");
    qmlRegisterUncreatableType<VisibilityMode>(uri, 1, 0, "VisibilityMode", "VisibilityMode Enum");
}

void PvComponentsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    engine->addImageProvider("areaDetector", new ADImageProvider);
}

