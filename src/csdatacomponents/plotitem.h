#ifndef PLOTITEM_H
#define PLOTITEM_H

#include <QQuickPaintedItem>

class QCustomPlot;
class QCPPlotTitle;
class QCPGraph;
class QCPAxis;
class GraphItem;
class AxisItem;

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QColor foreground READ foreground WRITE setForeground NOTIFY foregroundChanged)
    Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)
    Q_PROPERTY(QQmlListProperty<GraphItem> graphs READ graphs)

public:
    explicit CustomPlotItem(QQuickItem *parent=0);
    ~CustomPlotItem();

    void componentComplete();

    QCustomPlot * plot() {return mPlot;}

    //properties
    QString title();
    void setTitle(QString title);
    QColor foreground();
    void setForeground(QColor color);
    QColor background();
    void setBackground(QColor color);

    // graphs
    QQmlListProperty<GraphItem> graphs();
    static int graphsCount(QQmlListProperty<GraphItem> *list);
    static GraphItem * graph(QQmlListProperty<GraphItem> *list, int n);
    static void appendGraph(QQmlListProperty<GraphItem> *list, GraphItem *item);
    static void clearGraphs(QQmlListProperty<GraphItem> *list);

    // methods
    Q_INVOKABLE GraphItem* addGraph(AxisItem* xAxis, AxisItem* yAxis);
    Q_INVOKABLE void replot();

    // implemented virtual functions
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    void paint( QPainter* painter );

signals:
    void foregroundChanged();
    void backgroundChanged();

protected slots:
    void onCustomReplot();

private:
    QCustomPlot *mPlot;
    QCPPlotTitle *mTitle;
    QList<GraphItem*> mGraphs;
};

class GraphItem : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_ENUMS(LineStyle)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle NOTIFY lineStyleChanged)
    Q_PROPERTY(QVariantList data READ data WRITE setData NOTIFY dataChanged)    
    Q_PROPERTY(AxisItem* x READ x WRITE setX)
    Q_PROPERTY(AxisItem* y READ y WRITE setY)

public:
    enum LineStyle {
        Point = 0,
        Line,
        Fill
    };

    explicit GraphItem(QObject *parent=0);

    void classBegin() {}
    void componentComplete() {}

    void init();

    //properties
    void setData(QVariantList data);
    QVariantList data();

    void setColor(QColor color);
    QColor color();

    void setLineStyle(LineStyle style);
    LineStyle lineStyle();

    AxisItem* x() {return mXAxis;}
    void setX(AxisItem *x) {mXAxis = x;}

    AxisItem* y() {return mYAxis;}
    void setY(AxisItem *y) {mYAxis = y;}

    //methods
    Q_INVOKABLE void setData(QVariantList x, QVariantList y);
    Q_INVOKABLE void setData(QVariantList x, QVariant y);
    Q_INVOKABLE void setData(QVariant x, QVariant y);
    Q_INVOKABLE void clearData();

signals:
    void dataChanged(GraphItem *item);
    void colorChanged();
    void lineStyleChanged();

private:
    QCPGraph *mGraph;
    QVector<double> mX;
    QVector<double> mY;
    QColor _color;
    LineStyle mLineStyle;
    AxisItem *mXAxis;
    AxisItem *mYAxis;
    friend class CustomPlotItem;
};

class AxisItem : public QObject, public QQmlParserStatus
{
    Q_OBJECT

    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(AxisType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(AxisScale scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QString dateFormat READ dateFormat WRITE setDateFormat)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(double rangeLower READ rangeLower WRITE setRangeLower NOTIFY rangeLowerChanged)
    Q_PROPERTY(double rangeUpper READ rangeUpper WRITE setRangeUpper NOTIFY rangeUpperChanged)
    Q_PROPERTY(int tickCount READ tickCount WRITE setTickCount)
    Q_PROPERTY(bool tickVisible READ tickVisible WRITE setTickVisible)
    Q_PROPERTY(bool autoScale READ autoScale WRITE setAutoScale)
    Q_ENUMS(AxisType)
    Q_ENUMS(AxisScale)

public:
    explicit AxisItem(QObject *parent=0);
    enum AxisType {
        Left   = 0x01,
        Right  = 0x02,
        Top    = 0x04,
        Bottom = 0x08,
    };
    enum AxisScale {
        Linear,
        Logrithmic,
        DateTime,
    };

    QCPAxis *axis() {return mAxis;}

    void classBegin() {}
    void componentComplete();

    void setVisible(bool visible);
    bool visible();

    void setLabel(QString label);
    QString label();

    void setRangeLower(double lower);
    double rangeLower();

    void setRangeUpper(double upper);
    double rangeUpper();

    void setType(AxisType type) {_type = type; emit typeChanged();}
    AxisType type() {return _type;}

    void setScale(AxisScale scale);
    AxisScale scale() {return _scale;}

    void setDateFormat(QString format);
    QString dateFormat() {return _dateFormat;}

    void setTickCount(int count);
    int tickCount() {return _tickCount;}

    void setTickVisible(bool visible);
    bool tickVisible() {return _tickVisible;}

    void setAutoScale(bool on);
    bool autoScale() {return _autoScale;}

    Q_INVOKABLE void rescale();

signals:
    void typeChanged();
    void scaleChanged();
    void visibleChanged();
    void labelChanged();
    void rangeLowerChanged();
    void rangeUpperChanged();

private:
    bool _visible;
    QString _label;
    double _lower;
    double _upper;
    AxisType _type;
    AxisScale _scale;
    QString _dateFormat;
    int _tickCount;
    bool _tickVisible;
    bool _autoScale;

    QCPAxis *mAxis;
    friend class GraphItem;
};

#endif // PLOTITEM_H
