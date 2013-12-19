#ifndef PLOTITEM_H
#define PLOTITEM_H

#include <QQuickPaintedItem>

class QCustomPlot;
class QCPPlotTitle;
class QCPGraph;
class GraphItem;

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel)
    Q_PROPERTY(QQmlListProperty<GraphItem> graphs READ graphs)

public:
    explicit CustomPlotItem(QQuickItem *parent=0);
    ~CustomPlotItem();

    void onComponentCompleted();

    Q_INVOKABLE void initPlot();

    //properties
    QString title();
    void setTitle(QString title);
    QString xLabel();
    void setXLabel(QString label);
    QString yLabel();
    void setYLabel(QString label);

    // graphs
    QQmlListProperty<GraphItem> graphs();
    static int graphsCount(QQmlListProperty<GraphItem> *list);
    static GraphItem * graph(QQmlListProperty<GraphItem> *list, int n);
    static void appendGraph(QQmlListProperty<GraphItem> *list, GraphItem *item);
    static void clearGraphs(QQmlListProperty<GraphItem> *list);

    // methods
    Q_INVOKABLE GraphItem *addGraph();
    Q_INVOKABLE void replot();

    // implemented virtual functions
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    void paint( QPainter* painter );

protected slots:
    void onCustomReplot();
    void onDataChanged(GraphItem*item);

private:
    QCustomPlot *mPlot;
    QCPPlotTitle *mTitle;
    QList<GraphItem*> mGraphs;
};

class GraphItem : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QVariantList data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit GraphItem(QObject *parent=0);

    void classBegin() {}
    void componentComplete() {}

    //properties
    void setData(QVariantList data);
    QVariantList data();

    void setColor(QColor color);
    QColor color();

    //methods
    Q_INVOKABLE void addData(double x, double y);
signals:
    void dataChanged(GraphItem *item);
    void colorChanged();

private:
    QCPGraph *mGraph;
    QVector<QPointF> mData;
    QColor mColor;
    friend class CustomPlotItem;
};

#endif // PLOTITEM_H
