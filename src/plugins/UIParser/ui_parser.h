#pragma once

#include <QTextStream>
#include <QXmlStreamReader>

#include <set>
#include <map>
#include <string>

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
    void layoutItemToQML(QTextStream &ostream, DomLayoutItem *i, int level=0);
    bool limitsToQML(QTextStream &ostream, DomProperty *w, int level=0);
    void rectToQML(QTextStream &ostream, DomRect *r, int level=0);
    bool sizePolicyToQML(QTextStream &ostream, DomProperty *v, int level=0);

    // graphics widgets
    void compositeToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void frameToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void graphicsToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void imageToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void labelToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void lineToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void polylineToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

    // monitior widgets
    void barToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void byteToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void cartesianPlotToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void indicatorToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void ledToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void meterToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void stripChartToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void textUpdateToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

    // control widgets
    void choiceButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void menuToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void messageButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void relatedDisplayToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void scriptButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void shellCommandToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void sliderToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void spinBoxToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void textEntryToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void toggleButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void waveTableToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void wheelSwitchToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

    // Qt standard widgets
    void qlabelToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void groupBoxToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void textEditToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void tabWidgetToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

    void layoutToQML(QTextStream &ostream, DomLayout*l, int level=0, DomLayoutItem*i=nullptr);
    void spacerToQML(QTextStream &ostream, DomSpacer*l, int level=0, DomLayoutItem*i=nullptr);
    void widgetToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    QList<DomWidget*> orderedChildWidgets(DomWidget *w);
    QList<DomProperty*> uniqueProperties(QList<DomProperty*> p);
    QString escapedSingleQuote(QString s);

private:
    std::string nameValueString;
    std::map<std::string, std::string> nameValueTable;
    std::set<std::string> unmatchedNames;

    /* file */
    std::string file;
    /* DomUI */
    DomUI *ui;
};
