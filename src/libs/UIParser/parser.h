#pragma once

#include <QTextStream>
#include <QXmlStreamReader>

#include <set>
#include <map>
#include <string>

class DomUI;
class DomColor;
class DomLayout;
class DomLayoutItem;
class DomRect;
class DomFont;
class DomWidget;

class UI
{
public:
    UI();
    ~UI();
    
    void parse(QXmlStreamReader &reader);
    void toQML(QTextStream& ostream);
    void toPartialQML(QTextStream &ostream);

    QString colorToQML(DomColor *v);
    void rectToQML(QTextStream &ostream, DomRect *r, int level=0);
    void fontToQML(QTextStream &ostream, DomFont *r, int level=0);
    void layoutToQML(QTextStream &ostream, DomLayout*l, int level=0);

    void tabWidgetToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);
    void widgetToQML(QTextStream &ostream, DomWidget*w, int level=0, DomLayoutItem*i=nullptr);

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

private:
    std::string nameValueString;
    std::map<std::string, std::string> nameValueTable;
    std::set<std::string> unmatchedNames;

    /* file */
    std::string file;
    /* DomUI */
    DomUI *ui;
};
