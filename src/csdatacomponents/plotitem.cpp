/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include "plotitem.h"
#include "qcustomplot.h"

static int calcLabelFontSize(QRect size)
{
    int fontHeight;
    int width;

    width = qMin(size.width(), size.height());
    if(width > 1000) {
        fontHeight = 18;
    } else if(width > 900) {
        fontHeight = 16;
    } else if(width > 750) {
        fontHeight = 14;
    } else if(width > 600) {
        fontHeight = 12;
    } else if(width > 400) {
        fontHeight = 10;
    } else {
        fontHeight = 8;
    }
    return fontHeight;
}

static int calcTitleFontSize(QRect size)
{
    int fontHeight;
    int width;

    width = qMin(size.width(), size.height());
    if(width > 1000) {
        fontHeight = 26;
    } else if(width > 900) {
        fontHeight = 24;
    } else if(width > 750) {
        fontHeight = 22;
    } else if(width > 600) {
        fontHeight = 20;
    } else if(width > 500) {
        fontHeight = 18;
    } else if(width > 400) {
        fontHeight = 16;
    } else if(width > 300) {
        fontHeight = 14;
    } else if(width > 250) {
        fontHeight = 12;
    } else if(width > 200) {
        fontHeight = 10;
    } else {
        fontHeight = 8;
    }
    return fontHeight;
}

CustomPlotItem::CustomPlotItem( QQuickItem* parent )
    : QQuickPaintedItem( parent ),
      mPlot(0),mTitle(0)
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::AllButtons );

    mPlot = new QCustomPlot(this);
    mPlot->plotLayout()->clear();
    mPlot->plotLayout()->addElement(0, 0, new QCPAxisRect(mPlot, false));
    mPlot->plotLayout()->setRowSpacing(0);

    connect(mPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot);

    mPlot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom |  QCP::iSelectPlottables);

    // create title but leave it out until it has content
    mTitle = new QCPTextElement(mPlot);
}

CustomPlotItem::~CustomPlotItem()
{
    mGraphs.clear();
    delete mPlot;
}

void CustomPlotItem::componentComplete()
{
    QQuickPaintedItem::componentComplete();
    
    foreach(QObject *child, children()) {
        if (qobject_cast<GraphItem*>(child)) {
            GraphItem *graph = qobject_cast<GraphItem*>(child);
            graph->init();
        }
        if (qobject_cast<ColorMapItem*>(child)) {
            ColorMapItem *colormap = qobject_cast<ColorMapItem*>(child);
            colormap->init();
        }
    }
    replot();
}

GraphItem *CustomPlotItem::addGraph(AxisItem *xAxis, AxisItem *yAxis)
{
    GraphItem *graph = new GraphItem(this);
    graph->setX(xAxis);
    graph->setY(yAxis);
    graph->init();
    mGraphs.append(graph);
    return graph;
}

QString CustomPlotItem::title()
{
    return mTitle->text();
}

void CustomPlotItem::setTitle(QString title)
{
    mTitle->setText(title);
    if (title.isEmpty()) {
        if (mTitle->layout() != NULL) {
            mPlot->plotLayout()->take(mTitle);
            mPlot->plotLayout()->simplify();
        }
    }
    else {
        if (mTitle->layout() == NULL) {
            mPlot->plotLayout()->insertRow(0);
            mPlot->plotLayout()->addElement(0, 0, mTitle);
        }
    }
    mPlot->replot();
}

QColor CustomPlotItem::foreground()
{
    return mTitle->textColor();
}

void CustomPlotItem::setForeground(QColor color)
{
    mTitle->setTextColor(color);
}

QColor CustomPlotItem::background()
{
    return QColor();
}

void CustomPlotItem::setBackground(QColor color)
{
    mPlot->setBackground(color);
    mPlot->replot();
}

QQmlListProperty<GraphItem> CustomPlotItem::graphs()
{
    return QQmlListProperty<GraphItem>(this, this,
                                       &CustomPlotItem::appendGraph,
                                       &CustomPlotItem::graphsCount,
                                       &CustomPlotItem::graph,
                                       &CustomPlotItem::clearGraphs);
}

int CustomPlotItem::graphsCount(QQmlListProperty<GraphItem> *list)
{
    CustomPlotItem *item = dynamic_cast<CustomPlotItem*>((QObject*)list->data);
    return item->mGraphs.length();
}

GraphItem * CustomPlotItem::graph(QQmlListProperty<GraphItem> *list, int n)
{
    CustomPlotItem *item = dynamic_cast<CustomPlotItem*>((QObject*)list->data);
    return item->mGraphs.at(n);
}

void CustomPlotItem::appendGraph(QQmlListProperty<GraphItem> *list, GraphItem *it)
{
    CustomPlotItem *item = dynamic_cast<CustomPlotItem*>((QObject*)list->data);
    item->mGraphs.append(it);
}

void CustomPlotItem::clearGraphs(QQmlListProperty<GraphItem> *list)
{
    CustomPlotItem *item = dynamic_cast<CustomPlotItem*>((QObject*)list->data);
    item->mGraphs.clear();
}

void CustomPlotItem::replot()
{
    mPlot->replot(QCustomPlot::rpQueuedReplot);
}

void CustomPlotItem::paint(QPainter *painter)
{
    mPlot->paint(painter);
}

void CustomPlotItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    if (mPlot == 0)
        return;
    int titleFont = calcTitleFontSize(newGeometry.toRect());
    mTitle->setFont(QFont("Courier", titleFont));

    int labelFont = calcLabelFontSize(newGeometry.toRect());
    foreach (QCPAxis *axis, mPlot->axisRect()->axes()) {
        axis->setLabelFont(QFont("Courier", labelFont));
        axis->setTickLabelFont(QFont("Courier", labelFont));
    }
    QResizeEvent re(newGeometry.size().toSize(), oldGeometry.size().toSize());
    mPlot->setRect(QRect(0, 0, newGeometry.width(), newGeometry.height()));
    mPlot->resizeEvent(&re);
    mPlot->replot();
}

void CustomPlotItem::mousePressEvent(QMouseEvent *event)
{
    mPlot->mousePressEvent(event);
}
void CustomPlotItem::mouseMoveEvent(QMouseEvent *event)
{
    mPlot->mouseMoveEvent(event);
}
void CustomPlotItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && event->modifiers() == Qt::NoModifier)
        emit contextMenuRequested();
    else
        mPlot->mouseReleaseEvent(event);
}
void CustomPlotItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    foreach(QObject *child, children()) {
        if (qobject_cast<AxisItem*>(child)) {
            AxisItem *axis = qobject_cast<AxisItem*>(child);
            axis->rescale();
        }
    }
    mPlot->replot();
}
void CustomPlotItem::wheelEvent(QWheelEvent *event)
{
    mPlot->wheelEvent(event);
}

void CustomPlotItem::onCustomReplot()
{
    update();
}

/*
 * Graph Item
 */

GraphItem::GraphItem(QObject *parent)
    : QObject(parent),mName(""),mGraph(0),mXAxis(0),mYAxis(0)
{
}

void GraphItem::init()
{
    CustomPlotItem *plot = qobject_cast<CustomPlotItem*>(parent());
    if (plot == 0)
        return;
    mGraph = plot->plot()->addGraph(mXAxis->axis(), mYAxis->axis());
    connect(mGraph, SIGNAL(selectionChanged(bool)), this, SLOT(selectionChanged(bool)));
    setName(mName);
    setColor(mColor);
    setLineStyle(mLineStyle);
    mGraph->setData(mX, mY);
}

void GraphItem::selectionChanged(bool selected)
{
    if (mGraph) {
        QCPAxisRect *axisRect = mGraph->keyAxis()->axisRect();
        if (selected) {
            axisRect->setRangeDragAxes(mGraph->keyAxis(), mGraph->valueAxis());
            axisRect->setRangeZoomAxes(mGraph->keyAxis(), mGraph->valueAxis());
        }
        else {
            axisRect->setRangeDragAxes(0, 0);
            axisRect->setRangeZoomAxes(0, 0);
        }
    }
}

QString GraphItem::name()
{
    return mName;
}
void GraphItem::setName(QString name)
{
    mName = name;
    if (mGraph) {
        mGraph->setName(name);
    }
    emit nameChanged();
}

QColor GraphItem::color()
{
    return mColor;
}
void GraphItem::setColor(QColor color)
{
    mColor = color;
    if (mGraph) {
        mGraph->setPen(color);
        mGraph->valueAxis()->setBasePen(color);
    }
    emit colorChanged();
}

GraphItem::LineStyle GraphItem::lineStyle()
{
    return mLineStyle;
}
void GraphItem::setLineStyle(LineStyle lineStyle)
{
    mLineStyle = lineStyle;
    if (mGraph) {
        switch (lineStyle) {
        case Point: {
            mGraph->setLineStyle(QCPGraph::lsNone);
            QCPScatterStyle ssStyle(QCPScatterStyle::ssDisc, 2);
            mGraph->setScatterStyle(ssStyle);
        }
            break;
        case Line:
            mGraph->setScatterStyle(QCPScatterStyle());
            mGraph->setLineStyle(QCPGraph::lsLine);
            mGraph->setBrush(Qt::NoBrush);
            break;
        case Fill:
            mGraph->setScatterStyle(QCPScatterStyle());
            mGraph->setLineStyle(QCPGraph::lsLine);
            mGraph->setBrush(mColor);
            break;
        }
    }
    emit lineStyleChanged();
}

QVariantList GraphItem::data()
{
    QVariantList list;
    for (int i=0; i<mX.length(); i++) {
        list.append(QPointF(mX[i], mY[i]));
    }
    return list;
}

void GraphItem::setData(QVariant x, QVariant y)
{
    // x and y can be either scalar or vector or empty
    mX.clear(); mY.clear();
    if (x.canConvert<QVariantList>())  {
        foreach(QVariant v, x.value<QSequentialIterable>()) {
            mX.append(v.toDouble());
        }
    }

    if (y.canConvert<QVariantList>())  {
        foreach(QVariant v, y.value<QSequentialIterable>()) {
            mY.append(v.toDouble());
        }
    }

    if (x.canConvert(QMetaType::Double)) {
        mX.fill(x.toReal(), mY.size());
    } else if (mX.isEmpty())
        for(int i=0; i<=mY.length(); i++)
            mX.append(i);


    if (y.canConvert(QMetaType::Double)) {
        mY.fill(y.toReal(), mX.size());
    } else if (mY.isEmpty())
        for(int i=0; i<=mX.length(); i++)
            mY.append(i);

    if (mX.size() > mY.size())
        mX.resize(mY.size());
    else if (mY.size() > mX.size())
        mY.resize(mX.size());

    if (mGraph) {
        mGraph->setData(mX, mY);
    }

    emit dataChanged();
}
void GraphItem::setData(QVariantList x, QVariant y)
{
    mX.clear();
    for (int i=0; i<x.length(); i++) {
        mX.append(x[i].toDouble());
    }
    mY.clear();
    if (y.canConvert<QVariantList>())  {
        foreach(QVariant v, y.value<QSequentialIterable>()) {
            mY.append(v.toDouble());
        }
    }
    if (mY.isEmpty())
        for(int i=0; i<=mX.length(); i++)
            mY.append(i);

    if (mX.size() > mY.size())
        mX.resize(mY.size());
    else if (mY.size() > mX.size())
        mY.resize(mX.size());

    if (mGraph) {
        mGraph->setData(mX, mY);
    }

    emit dataChanged();
}

void GraphItem::setData(QVariantList x, QVariantList y)
{
    mX.clear(); mY.clear();
    for (int i=0; i<qMin(x.length(), y.length()); i++) {
        mX.append(x[i].toDouble());
        mY.append(y[i].toDouble());
    }
    if (mGraph) {
        mGraph->setData(mX, mY);
    }

    emit dataChanged();
}

void GraphItem::setData(QVariantList data)
{
    mX.clear();mY.clear();
    foreach(QVariant v, data) {
        QPointF pt = v.toPointF();
        mX.append(pt.x());
        mY.append(pt.y());
    }
    if (mGraph) {
        mGraph->setData(mX, mY);
    }

    emit dataChanged();
}

void GraphItem::clearData()
{
    mX.clear();mY.clear();
    if (mGraph) {
        mGraph->data()->clear();
    }

    emit dataChanged();
}

/*
 * ColorMap Item
 */

ColorMapItem::ColorMapItem(QObject *parent)
    : QObject(parent), mColorMap(0),mColorScale(0),mXAxis(0),mYAxis(0),_interpolate(false)
{
}

void ColorMapItem::init()
{
    CustomPlotItem *plot = qobject_cast<CustomPlotItem*>(parent());
    if (plot == 0)
        return;
    mColorMap = new QCPColorMap(mXAxis->axis(), mYAxis->axis());
    mColorMap->setInterpolate(_interpolate);

    mColorScale = new QCPColorScale(plot->plot());
    mColorScale->setType(QCPAxis::atRight);
    mColorScale->setGradient(QCPColorGradient::gpHot);
    mColorMap->setColorScale(mColorScale);

    QCPMarginGroup *marginGroup = new QCPMarginGroup(plot->plot());
    plot->plot()->axisRect()->setMarginGroup(QCP::msTop | QCP::msBottom, marginGroup);
    mColorScale->setMarginGroup(QCP::msTop | QCP::msBottom, marginGroup);

    plot->plot()->plotLayout()->addElement(0, 1, mColorScale);
}

bool ColorMapItem::interpolate()
{
    return _interpolate;
}

void ColorMapItem::setInterpolate(bool enabled)
{
    _interpolate = enabled;
    if (mColorMap != 0)
        mColorMap->setInterpolate(enabled);
}

void ColorMapItem::setXRange(double lower, double upper, int size)
{
    if (mColorMap) {
        mColorMap->data()->setKeySize(size);
        mColorMap->data()->setKeyRange(QCPRange(lower, upper));
        mXAxis->rescale();
    }
}

void ColorMapItem::setYRange(double lower, double upper, int size)
{
    if (mColorMap) {
        mColorMap->data()->setValueSize(size);
        mColorMap->data()->setValueRange(QCPRange(lower, upper));
        mYAxis->rescale();
    }
}

void ColorMapItem::setData(double x, double y, double z)
{
    if (mColorMap) {
        mColorMap->data()->setData(x, y, z);
        mColorMap->rescaleDataRange(true);
    }
}

void ColorMapItem::setCell(int x, int y, double z)
{
    if (mColorMap) {
        mColorMap->data()->setCell(x, y, z);
        mColorMap->rescaleDataRange(true);
    }
}

void ColorMapItem::clearData()
{
    if (mColorMap) {
        mColorMap->data()->clear();
    }
}

/*
 * Axis Item
 */

AxisItem::AxisItem(QObject *parent)
    : QObject(parent), mAxis(0)
{
    _type = Left;
    _scale = Linear;
    _dateFormat = "hh:mm:ss";
    _visible = true;
    _label = "";
    _lower = 0;
    _upper = 1;
    _tickCount = 3;
    _tickVisible = true;
    _autoScale = true;
}

void AxisItem::componentComplete()
{
    CustomPlotItem *plot = qobject_cast<CustomPlotItem*>(parent());
    if (plot == 0)
        return;
    mAxis = plot->plot()->axisRect()->addAxis(QCPAxis::AxisType(_type));

    int labelFont = calcLabelFontSize(mAxis->axisRect()->toRect());
    mAxis->setLabelFont(QFont("Courier", labelFont));
    mAxis->setTickLabelFont(QFont("Courier", labelFont));

    setScale(_scale);
    setDateFormat(_dateFormat);
    setTickCount(_tickCount);
    setTickVisible(_tickVisible);
    setVisible(_visible);
    setLabel(_label);
    setRangeLower(_lower);
    setRangeUpper(_upper);
    setAutoScale(_autoScale);
}

bool AxisItem::visible()
{
    return _visible;
}
void AxisItem::setVisible(bool visible)
{
    _visible = visible;
    if (mAxis)
        mAxis->setVisible(visible);
}

void AxisItem::setTickVisible(bool visible)
{
    _tickVisible = visible;
    if (mAxis) {
        mAxis->setTicks(visible);
        mAxis->setTickLabels(visible);
    }
}

QString AxisItem::label()
{
    return _label;
}
void AxisItem::setLabel(QString label)
{
    _label = label;
    if (mAxis) {
        mAxis->setLabel(label);
        mAxis->parentPlot()->replot();
    }
    emit labelChanged();
}

double AxisItem::rangeLower()
{
    return _lower;
}
void AxisItem::setRangeLower(double lower)
{
    if (_lower == lower)
        return;
    _lower = lower;

    if (mAxis && !autoScale()) {
        mAxis->setRangeLower(lower);
        mAxis->parentPlot()->replot();
    }
    emit rangeLowerChanged();
}

double AxisItem::rangeUpper()
{
    return _upper;
}
void AxisItem::setRangeUpper(double upper)
{
    if (_upper == upper)
        return;
    _upper = upper;

    if (mAxis && !autoScale()) {
        mAxis->setRangeUpper(upper);
        mAxis->parentPlot()->replot();
    }
    emit rangeUpperChanged();
}

void AxisItem::setScale(AxisScale scale)
{
    _scale = scale;
    if (mAxis) {
        switch(scale) {
        case Linear:
            mAxis->setScaleType(QCPAxis::stLinear);
            break;
        case Logrithmic:
            mAxis->setScaleType(QCPAxis::stLogarithmic);
            break;
        case DateTime:
            mAxis->setTicker(QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime));
            break;
        }
    }
    emit scaleChanged();
}

void AxisItem::setDateFormat(QString format)
{
    _dateFormat = format;
    if (mAxis) {
        QSharedPointer<QCPAxisTickerDateTime> ticker = mAxis->ticker().dynamicCast<QCPAxisTickerDateTime>();
        if (!ticker.isNull())
            ticker->setDateTimeFormat(format);
    }
}

void AxisItem::setTickCount(int count)
{
    _tickCount = count;
    if (mAxis) {
        mAxis->ticker()->setTickCount(count);
    }
}

void AxisItem::setAutoScale(bool on)
{
    _autoScale = on;

    if (!mAxis)
        return;

    if (_autoScale)
        mAxis->rescale();
    else
        mAxis->setRange(_lower, _upper);

    mAxis->parentPlot()->replot();
}

void AxisItem::rescale()
{
    if (!mAxis)
        return;

    if (autoScale())
        mAxis->rescale();
    else
        mAxis->setRange(_lower, _upper);

    mAxis->parentPlot()->replot();
}
