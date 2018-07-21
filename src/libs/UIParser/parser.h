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
class DomWidget;

class UI
{
public:
    UI();
    ~UI();
    
    void parse(QXmlStreamReader &reader);
    void toQML(QTextStream& ostream);
    void toPartialQML(QTextStream &ostream);

    void setFileName(std::string filename) { file = filename;}
    std::string fileName() { return file; }

    void setMacros(std::map<std::string, std::string> macros) {
        nameValueTable = macros;
        // convert to macro string form
        nameValueString.clear();
        for (std::map<std::string,std::string>::iterator it=nameValueTable.begin(); it!=nameValueTable.end(); ++it)
            nameValueString += it->first + '=' + it->second + ',';
    }

    bool findMacro(std::string macro) {
        return nameValueTable.find(macro) != nameValueTable.end();
    }

    std::string macroValue(std::string macro) {
        return nameValueTable.at(macro);
    }

    std::string macroString() {return nameValueString;}

    std::set<std::string> unmatchedMacro() {
        return unmatchedNames;
    }

    void addUnmatchedMacro(std::string macro) {
        unmatchedNames.insert(macro);
    }

protected:
    // helper methods to convert standard properties to QML
    QString colorToQML(DomColor *v);
    QString directionToQML(QString direction);
    QString formatToQML(QString format);
    QString labelStyleToQML(QString style);

    bool dynamicAttributeToQML(QTextStream &ostream, DomProperty *w, int level=0);
    bool limitsToQML(QTextStream &ostream, DomProperty *w, int level=0);
    void rectToQML(QTextStream &ostream, DomRect *r, int level=0);
    void fontToQML(QTextStream &ostream, DomFont *r, int level=0);
    void layoutToQML(QTextStream &ostream, DomLayout*l, int level=0);

    // graphics widgets
    void compositeToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void frameToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void graphicsToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void imageToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void labelToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void polylineToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

    // monitior widgets
    void barToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void byteToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void indicatorToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void meterToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void stripChartToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void textUpdateToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

    // control widgets
    void choiceButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void menuToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void messageButtonToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void relatedDisplayToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void shellCommandToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void sliderToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void spinBoxToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void textEntryToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void wheelSwitchToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

    // Qt standard widgets
    void tabWidgetToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

    void widgetToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    QVector<DomWidget*> orderedChildWidgets(DomWidget *w);

private:
    std::string nameValueString;
    std::map<std::string, std::string> nameValueTable;
    std::set<std::string> unmatchedNames;

    /* file */
    std::string file;
    /* DomUI */
    DomUI *ui;
};
