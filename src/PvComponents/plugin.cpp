/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "plugin.h"

#include "pvobject.h"
#include "shapes.h"
#include "enums.h"
#include "utils.h"
#include "adimageprovider.h"
#include "plotitem.h"
#include "baseitem.h"
#include "basewindow.h"
#include "windowmanager.h"

#include <qqml.h>

static QObject *utils_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return new QCSUtils();
}

static QObject *windowmanager_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return new WindowManager();
}

void PvComponentsPlugin::registerTypes(const char *uri)
{
    // @uri PvComponents
    qmlRegisterType<QQuickPvObject>(uri, 1, 0, "PvObject");
    qmlRegisterType<BaseItem>(uri, 1, 0, "BaseItem");
    qmlRegisterType<PolylineItem>(uri, 1, 0, "Polyline");
    qmlRegisterType<PolygonItem>(uri, 1, 0, "Polygon");
    qmlRegisterType<ArcItem>(uri, 1, 0, "Arc");
    qmlRegisterType<PaintedRectangletem>(uri, 1, 0, "PaintedRectangle");
    qmlRegisterType<OvalItem>(uri, 1, 0, "Oval");
    qmlRegisterType<DoubleRectItem>(uri, 1, 0, "DoubleRect");
    qmlRegisterType<CustomPlotItem>(uri, 1, 0, "Plot");
    qmlRegisterType<GraphItem>(uri, 1, 0, "Graph");
    qmlRegisterType<AxisItem>(uri, 1, 0, "Axis");
    qmlRegisterType<BaseWindow>(uri, 1, 0, "BaseWindow");
    qmlRegisterSingletonType<QCSUtils>(uri, 1, 0, "Utils", utils_provider);
    qmlRegisterSingletonType<WindowManager>(uri, 1, 0, "WindowManager", windowmanager_provider);
    qmlRegisterUncreatableType<FrameShadow>(uri, 1, 0, "FrameShadow", "FrameShadow Enum");
    qmlRegisterUncreatableType<TextFormat>(uri, 1, 0, "TextFormat", "TextFomat Enum");
    qmlRegisterUncreatableType<ColorMode>(uri, 1, 0, "ColorMode", "ColorMode Enum");
    qmlRegisterUncreatableType<Direction>(uri, 1, 0, "Direction", "Direction Enum");
    qmlRegisterUncreatableType<EdgeStyle>(uri, 1, 0, "EdgeStyle", "EdgeStyle Enum");
    qmlRegisterUncreatableType<FillStyle>(uri, 1, 0, "FillStyle", "FillStyle Enum");
    qmlRegisterUncreatableType<VisibilityMode>(uri, 1, 0, "VisibilityMode", "VisibilityMode Enum");
    qmlRegisterUncreatableType<LimitsSource>(uri, 1, 0, "LimitsSource", "LimitsSource Enum");
    qmlRegisterUncreatableType<RelatedDisplayVisual>(uri, 1, 0, "RelatedDisplayVisual", "RelatedDisplayVisual Enum");
    qmlRegisterUncreatableType<RelatedDisplayMode>(uri, 1, 0, "RelatedDisplayMode", "RelatedDisplayMode Enum");
    qmlRegisterUncreatableType<LabelStyle>(uri, 1, 0, "LabelStyle", "LabelStyle Enum");
    qmlRegisterUncreatableType<TimeUnit>(uri, 1, 0, "TimeUnit", "TimeUnit Enum");
    qmlRegisterUncreatableType<PlotStyle>(uri, 1, 0, "PlotStyle", "PlotStyle Enum");
    qmlRegisterUncreatableType<Stacking>(uri, 1, 0, "Stacking", "Stacking Enum");
    qmlRegisterUncreatableType<FillMode>(uri, 1, 0, "FillMode", "FillMode Enum");
    qmlRegisterUncreatableType<EraseMode>(uri, 1, 0, "EraseMode", "EraseMode Enum");
    qmlRegisterUncreatableType<CartesianPlotTimeFormat>(uri, 1, 0, "CartesianPlotTimeFormat", "CartesianPlotTimeFormat Enum");
    qmlRegisterUncreatableType<CartesianPlotRangeStyle>(uri, 1, 0, "CartesianPlotRangeStyle", "CartesianPlotRangeStyle Enum");
    qmlRegisterUncreatableType<CartesianPlotYAxis>(uri, 1, 0, "CartesianPlotYAxis", "CartesianPlotYAxis Enum");
    qmlRegisterUncreatableType<CartesianPlotAxisStyle>(uri, 1, 0, "CartesianPlotAxisStyle", "CartesianPlotAxisStyle Enum");
}

void PvComponentsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    engine->addImageProvider("areaDetector", new ADImageProvider);
}

