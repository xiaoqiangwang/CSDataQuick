#include "plotitem.h"
#include "qcustomplot.h"

CustomPlotItem::CustomPlotItem( QQuickItem* parent )
    : QQuickPaintedItem( parent ),
      mPlot(0),mTitle(0)
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::AllButtons );

    mPlot = new QCustomPlot(this);
    mPlot->setFont(QFont("Courier", 10));

    mPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    mPlot->xAxis->setDateTimeFormat("hh:mm:ss");

    mPlot->legend->setVisible(true);

    connect(mPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot);

    mPlot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );

    // create and insert title
    mTitle = new QCPPlotTitle(mPlot);
    mPlot->plotLayout()->insertRow(0);
    mPlot->plotLayout()->addElement(0, 0, mTitle);
}

CustomPlotItem::~CustomPlotItem()
{
    mGraphs.clear();
    delete mPlot;
}

void CustomPlotItem::onComponentCompleted()
{
    qDebug() << "here i am";
}

void CustomPlotItem::initPlot()
{
    foreach(QObject *child, children()) {
        if (qobject_cast<GraphItem*>(child)) {
            GraphItem *item = qobject_cast<GraphItem*>(child);
            mGraphs.append(item);
            QCPGraph *graph = mPlot->addGraph();
            QVector<double> x;
            QVector<double> y;
            foreach (QPointF pt, item->mData) {
                x.append(pt.x());
                y.append(pt.y());
            }
            graph->setData(x, y);
            graph->setPen(item->color());
            connect(item, SIGNAL(dataChanged(GraphItem*)), this, SLOT(onDataChanged(GraphItem*)));
        }
    }
}

void CustomPlotItem::onDataChanged(GraphItem* item)
{
    QVector<double> x;
    QVector<double> y;
    foreach (QPointF pt, item->mData) {
        x.append(pt.x());
        y.append(pt.y());
    }
    item->mGraph->setData(x, y);
}

QString CustomPlotItem::title()
{
    return mTitle->text();
}

void CustomPlotItem::setTitle(QString title)
{
    mTitle->setText(title);
    mPlot->replot();
}

QString CustomPlotItem::xLabel()
{
    return mPlot->xAxis->label();
}

void CustomPlotItem::setXLabel(QString label)
{
    mPlot->xAxis->setLabel(label);
}

QString CustomPlotItem::yLabel()
{
    return mPlot->yAxis->label();
}

void CustomPlotItem::setYLabel(QString label)
{
    mPlot->yAxis->setLabel(label);
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

/*
 *  Public methods
 */
GraphItem* CustomPlotItem::addGraph()
{
    QCPGraph * graph = mPlot->addGraph();
    GraphItem *item = new GraphItem(this);
    item->mGraph = graph;
    mGraphs.append(item);
    return item;
}

void CustomPlotItem::replot()
{
    mPlot->replot();
}

void CustomPlotItem::paint(QPainter *painter)
{
    if (mPlot)
        mPlot->paint(painter);
}

void CustomPlotItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QResizeEvent re(newGeometry.size().toSize(), oldGeometry.size().toSize());
    mPlot->setRect(QRect(0, 0, newGeometry.width(), newGeometry.height()));
    mPlot->resizeEvent(&re);
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

GraphItem::GraphItem(QObject *parent)
    : QObject(parent)
{
}

QColor GraphItem::color()
{
    return mGraph->pen().color();
}

void GraphItem::setColor(QColor color)
{
    mGraph->setPen(color);
}

QVariantList GraphItem::data()
{
    QVariantList list;
    foreach(QPointF pt, mData) {
        list.append(pt.toPoint());
    }
    return list;
}

void GraphItem::setData(QVariantList data)
{
    mData.clear();
    QVector<double> x, y;
    foreach(QVariant v, data) {
        QPointF pt = v.toPointF();
        mData.append(pt);
        x.append(pt.x());
        y.append(pt.y());
    }
    mGraph->setData(x, y);
    mGraph->parentPlot()->xAxis->rescale();
    mGraph->parentPlot()->yAxis->rescale(true);
}

void GraphItem::addData(double x, double y)
{
    mGraph->addData(x, y);
    mGraph->parentPlot()->xAxis->rescale();
    mGraph->parentPlot()->yAxis->rescale(true);
    //mGraph->parentPlot()->xAxis->setRange(x + 0.25, 8, Qt::AlignRight);
}
