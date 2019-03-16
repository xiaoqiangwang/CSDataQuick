#ifndef PLOTITEM_H
#define PLOTITEM_H

#include <QQuickPaintedItem>

#include "qcustomplot.h"

class GraphItem;
class AxisItem;
class LayoutElement;
class CustomPlotItemAttached;

class ItemProxy
{
public:
    virtual ~ItemProxy();

    virtual void init() = 0;
};

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QColor foreground READ foreground WRITE setForeground NOTIFY foregroundChanged)
    Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows)
    Q_PROPERTY(int columns READ columns WRITE setColumns)
    Q_PROPERTY(bool legendVisible READ legendVisible WRITE setLegendVisible NOTIFY legendVisibleChanged)
    Q_PROPERTY(QQmlListProperty<GraphItem> graphs READ graphs)

public:
    explicit CustomPlotItem(QQuickItem *parent=Q_NULLPTR);
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
    int rows();
    void setRows(int);
    int columns();
    void setColumns(int);
    bool legendVisible();
    void setLegendVisible(bool visible);

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

    static CustomPlotItemAttached *qmlAttachedProperties(QObject *);

signals:
    void foregroundChanged();
    void backgroundChanged();
    void legendVisibleChanged();
    void contextMenuRequested();

protected slots:
    void onCustomReplot();

private:
    QCustomPlot *mPlot;
    QCPTextElement *mTitle;
    QList<GraphItem*> mGraphs;
};

class CustomPlotItemAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow)
    Q_PROPERTY(double column READ column WRITE setColumn)
    Q_PROPERTY(int rowStretch MEMBER _rowStretch WRITE setRowStretch)
    Q_PROPERTY(double columnStretch MEMBER _columnStretch WRITE setColumnStretch)

public:
    CustomPlotItemAttached(QObject *object);

    int row() { return _row;}
    void setRow(int row);
    int column() { return _column;}
    void setColumn(int column);
    void setRowStretch(double stretch);
    void setColumnStretch(double stretch);

protected:
    LayoutElement *item();
    CustomPlotItem *plot();

private:
    int _row, _column;
    double _rowStretch, _columnStretch;
};

QML_DECLARE_TYPEINFO(CustomPlotItem, QML_HAS_ATTACHED_PROPERTIES)


class GraphItem : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
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
    Q_ENUM(LineStyle)

    explicit GraphItem(QObject *parent=Q_NULLPTR);

    void classBegin() {}
    void componentComplete() {}

    void init();

    //properties
    void setName(QString name);
    QString name();

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

public slots:
    void selectionChanged(bool selected);

signals:
    void dataChanged();
    void nameChanged();
    void colorChanged();
    void lineStyleChanged();

private:
    QCPGraph *mGraph;
    QVector<double> mX;
    QVector<double> mY;
    QString mName;
    QColor mColor;
    LineStyle mLineStyle;
    AxisItem *mXAxis;
    AxisItem *mYAxis;
    friend class CustomPlotItem;
};

class ColorMapItem : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(AxisItem* x READ x WRITE setX)
    Q_PROPERTY(AxisItem* y READ y WRITE setY)
    Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate)

public:

    explicit ColorMapItem(QObject *parent=Q_NULLPTR);

    void classBegin() {}
    void componentComplete() {}

    void init();

    //properties
    AxisItem* x() {return mXAxis;}
    void setX(AxisItem *x) {mXAxis = x;}

    AxisItem* y() {return mYAxis;}
    void setY(AxisItem *y) {mYAxis = y;}

    bool interpolate();
    void setInterpolate(bool enabled);

    //methods
    Q_INVOKABLE void setXRange(double lower, double upper, int size);
    Q_INVOKABLE void setYRange(double lower, double upper, int size);
    Q_INVOKABLE void setData(double x, double y, double z);
    Q_INVOKABLE void setCell(int x, int y, double z);
    Q_INVOKABLE void clearData();

signals:

private:
    QCPColorMap *mColorMap;
    QCPColorScale *mColorScale;
    AxisItem *mXAxis;
    AxisItem *mYAxis;
    bool _interpolate;
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
    Q_PROPERTY(bool autoScale READ autoScale WRITE setAutoScale NOTIFY autoScaleChanged)

public:
    explicit AxisItem(QObject *parent=Q_NULLPTR);
    enum AxisType {
        Left   = 0x01,
        Right  = 0x02,
        Top    = 0x04,
        Bottom = 0x08,
    };
    Q_ENUM(AxisType)
    enum AxisScale {
        Linear,
        Logrithmic,
        DateTime,
    };
    Q_ENUM(AxisScale)

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
    void autoScaleChanged();

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


class MarginGroup : public QObject, public ItemProxy
{
    Q_OBJECT
    Q_PROPERTY(MarginSides sides READ sides WRITE setSides NOTIFY sidesChanged)

public:
    explicit MarginGroup(QObject *parent=Q_NULLPTR);

    enum MarginSide {
        Left = QCP::msLeft,
        Right = QCP::msRight,
        Top = QCP::msTop,
        Bottom = QCP::msBottom,
        All = QCP::msAll,
        None = QCP::msNone
    };
    Q_DECLARE_FLAGS(MarginSides, MarginSide)
    Q_ENUM(MarginSide)
    Q_FLAG(MarginSides)

    MarginSides sides();
    void setSides(MarginSides sides);

    void init();
    QCPMarginGroup *group();
signals:
    void sidesChanged();

private:
    MarginSides _sides;
    QCPMarginGroup *_marginGroup;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MarginGroup::MarginSides)

class LayoutElement : public QObject, public ItemProxy
{
    Q_OBJECT
    Q_PROPERTY(MarginGroup * leftMarginGroup MEMBER _leftMarginGroup WRITE setLeftMarginGroup)
    Q_PROPERTY(MarginGroup * rightMarginGroup MEMBER _rightMarginGroup WRITE setRightMarginGroup)
    Q_PROPERTY(MarginGroup * topMarginGroup MEMBER _topMarginGroup WRITE setTopMarginGroup)
    Q_PROPERTY(MarginGroup * bottomMarginGroup MEMBER _bottomMarginGroup WRITE setBottomMarginGroup)
public:
    explicit LayoutElement(QObject *parent=Q_NULLPTR);

    void setLeftMarginGroup(MarginGroup *);
    void setRightMarginGroup(MarginGroup *);
    void setTopMarginGroup(MarginGroup *);
    void setBottomMarginGroup(MarginGroup *);

    virtual void init();
    QCPLayoutElement *element();

signals:

protected:
    MarginGroup *_leftMarginGroup;
    MarginGroup *_rightMarginGroup;
    MarginGroup *_topMarginGroup;
    MarginGroup *_bottomMarginGroup;
    QCPLayoutElement *_element;
};

class AxisRect : public LayoutElement
{
    Q_OBJECT
public:
    explicit AxisRect(QObject *parent=Q_NULLPTR);

    void init();
signals:

private:
};

#endif // PLOTITEM_H
