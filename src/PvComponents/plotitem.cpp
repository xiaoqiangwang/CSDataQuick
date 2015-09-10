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

    mPlot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );

    // create title but leave it out until it has content
    mTitle = new QCPPlotTitle(mPlot);
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
    }
    replot();
}

GraphItem *CustomPlotItem::addGraph(AxisItem *xAxis, AxisItem *yAxis)
{
    GraphItem *graph = new GraphItem(this);
    graph->setX(xAxis);
    graph->setY(yAxis);
    graph->init();
    return graph;
}

QString CustomPlotItem::title()
{
    return mTitle->text();
}

void CustomPlotItem::setTitle(QString title)
{
    mTitle->setText(title);
    if (title.isEmpty())
        mPlot->plotLayout()->take(mTitle);
    else {
        mPlot->plotLayout()->insertRow(0);
        mPlot->plotLayout()->addElement(0, 0, mTitle);
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
    mPlot->replot();
}

void CustomPlotItem::paint(QPainter *painter)
{
    mPlot->paint(painter);
}

void CustomPlotItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
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
    mPlot->mouseReleaseEvent(event);
}
void CustomPlotItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    mPlot->mouseDoubleClickEvent(event);
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
    : QObject(parent), mGraph(0),mXAxis(0),mYAxis(0)
{
}

void GraphItem::init()
{
    CustomPlotItem *plot = qobject_cast<CustomPlotItem*>(parent());
    if (plot == 0)
        return;
    mGraph = plot->plot()->addGraph(mXAxis->axis(), mYAxis->axis());
    setColor(_color);
    mGraph->setData(mX, mY);
}

QColor GraphItem::color()
{
    return _color;
}
void GraphItem::setColor(QColor color)
{
    _color = color;
    if (mGraph) {
        mGraph->setPen(color);
        mGraph->valueAxis()->setBasePen(color);
    }
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

    if (mX.isEmpty())
        for(int i=0; i<=mY.length(); i++)
            mX.append(i);

    if (mY.isEmpty())
        for(int i=0; i<=mX.length(); i++)
            mY.append(i);

    if (mGraph) {
        mGraph->setData(mX, mY);
    }
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
}

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
}

void AxisItem::componentComplete()
{
    CustomPlotItem *plot = qobject_cast<CustomPlotItem*>(parent());
    if (plot == 0)
        return;
    mAxis = plot->plot()->axisRect()->addAxis(QCPAxis::AxisType(_type));
    mAxis->setAutoTicks(true);
    mAxis->setAutoTickLabels(true);

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
    if (mAxis)
        mAxis->setLabel(label);
    emit labelChanged();
}

double AxisItem::rangeLower()
{
    return _lower;
}
void AxisItem::setRangeLower(double lower)
{
    _lower = lower;
    if (mAxis)
        mAxis->setRangeLower(lower);
    emit rangeLowerChanged();
}

double AxisItem::rangeUpper()
{
    return _upper;
}
void AxisItem::setRangeUpper(double upper)
{
    _upper = upper;
    if (mAxis)
        mAxis->setRangeUpper(upper);
    emit rangeUpperChanged();
}

void AxisItem::setScale(AxisScale scale)
{
    _scale = scale;
    if (mAxis) {
        switch(scale) {
        case Linear:
            mAxis->setTickLabelType(QCPAxis::ltNumber);
            mAxis->setScaleType(QCPAxis::stLinear);
            break;
        case Logrithmic:
            mAxis->setTickLabelType(QCPAxis::ltNumber);
            mAxis->setScaleType(QCPAxis::stLinear);
            break;
        case DateTime:
            mAxis->setTickLabelType(QCPAxis::ltDateTime);
            break;
        }
    }
    emit scaleChanged();
}

void AxisItem::setDateFormat(QString format)
{
    _dateFormat = format;
    if (mAxis) {
        mAxis->setDateTimeFormat(format);
    }
}

void AxisItem::setTickCount(int count)
{
    _tickCount = count;
    if (mAxis) {
        mAxis->setAutoTickCount(count);
    }
}
