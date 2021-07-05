#pragma once

#include <QTextStream>
#include <QXmlStreamReader>

#include <set>
#include <map>
#include <string>

namespace UI {

class DomUI;
class DomColor;
class DomFont;
class DomLayout;
class DomLayoutItem;
class DomProperty;
class DomRect;
class DomSpacer;
class DomWidget;

class UI
{
public:
    UI();
    ~UI();
    
    void parse(QXmlStreamReader &reader);
    void toQML(QTextStream& ostream);
    void toPartialQML(QTextStream &ostream);

protected:
    // helper methods to convert standard properties to QML
    QString colorToQML(DomColor *v);
    QString cssColorToQML(QString css, QString role);
    QString directionToQML(QString direction);
    QString formatToQML(QString format);
    QString labelStyleToQML(QString style);

    bool dynamicAttributeToQML(QTextStream &ostream, DomProperty *w, int level=0);
    void fontToQML(QTextStream &ostream, DomFont *r, int level=0);
    void layoutItemToQML(QTextStream &ostream, DomLayoutItem *i, DomWidget *w, QString hSizeType, QString vSizeType, int level=0);
    bool limitsToQML(QTextStream &ostream, DomProperty *w, int level=0);
    void rectToQML(QTextStream &ostream, DomRect *r, int level=0);

    // graphics widgets
    void compositeToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void frameToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void graphicsToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void imageToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void labelToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void lineToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void polylineToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);

    // monitior widgets
    void barToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void byteToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void cartesianPlotToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void indicatorToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void ledToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void meterToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void stripChartToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void textUpdateToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);

    // control widgets
    void choiceButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void menuToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void messageButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void relatedDisplayToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void scriptButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void shellCommandToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void sliderToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void spinBoxToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void textEntryToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void toggleButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void waveTableToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void wheelSwitchToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);

    // Qt standard widgets
    void qlabelToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void groupBoxToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void textEditToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void scrollAreaToQML(QTextStream& ostream, DomWidget *w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void tabWidgetToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);

    void layoutToQML(QTextStream &ostream, DomLayout*l, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void spacerToQML(QTextStream &ostream, DomSpacer*l, int level=0, DomLayoutItem*i=Q_NULLPTR);
    void widgetToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=Q_NULLPTR);
    QList<DomWidget*> orderedChildWidgets(DomWidget *w);
    QList<DomProperty*> uniqueProperties(QList<DomProperty*> p);
    QString escapedSingleQuote(QString s);
    QStringList split(QString s, QChar sep);
    QString uncapitalize(QString s);

private:
    std::string nameValueString;
    std::map<std::string, std::string> nameValueTable;
    std::set<std::string> unmatchedNames;

    /* file */
    std::string file;
    /* DomUI */
    DomUI *ui;
};

} // namespace UI
