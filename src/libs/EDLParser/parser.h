#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <string>

enum ObjectType {
    EL_Object = 0,
    EL_Screen,

    EL_Arc,
    EL_Circle,
    EL_Image,
    EL_Lines,
    EL_Rectangle,
    EL_Symbols,
    EL_Text,

    EL_Bar,
    EL_Byte,
    EL_CartesianPlot,
    EL_Indicator,
    EL_MessageBox,
    EL_Meter,
    EL_StripChart,
    EL_TextUpdate,

    EL_TextEntry,
    EL_Slider,
    EL_Button,
    EL_Menu,
    EL_ChoiceButton,
    EL_RadioBox,
    EL_MessageButton,
    EL_RelatedDisplay,
    EL_ShellCommand,
    EL_UpDownButton,
    EL_WheelSwitch,

    EL_ExitButton,
    EL_Composite
};

/*
 * types for parsing
 */
typedef enum
{ T_WORD, T_COMMENT, T_NEWLINE, T_QUOTE, T_LEFT_BRACE, T_RIGHT_BRACE, T_EOF} TOKEN;

#define MAX_TOKEN_LENGTH        256     /* max size of strings in edl    */


/* Auxilary Classes */
typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Rect;

class Screen;
class Object
{
public:
    Object(ObjectType type, Object *parent) {
        _level = 0;
        _parent = parent;
        _type = type;
        _version = 0;
        /* find out the depth in the element tree */
        Object *p = parent;
        if (p == 0) _screen = (Screen *)this;
        while (p) {
            _level ++;
            if (p->parent() == 0)
                _screen = (Screen *)p;
            p = p->parent();
        }
    }
    virtual ~Object() {}

    ObjectType type() const {return _type;}
    Object *parent() const {return _parent;}
    Screen *screen() const {return _screen;}

    int level() const {return _level;}

    Rect rect() const {return _rect; }
    void setRect(Rect rect) { this->_rect = rect;}

    virtual void parse(std::istream& fstream);
    void parsePropertyValue(std::istream &fstream);
    void parseGroup(std::istream& fstream);


    void arcToQML(std::ostream& ostream);
    void circleToQML(std::ostream& ostream);
    void imageToQML(std::ostream& ostream);
    void linesToQML(std::ostream& ostream);
    void rectangleToQML(std::ostream& ostream);
    void relatedDisplayToQML(std::ostream& ostream);
    void textToQML(std::ostream& ostream);

    void barToQML(std::ostream& ostream);
    void byteToQML(std::ostream& ostream);
    void indicatorToQML(std::ostream& ostream);
    //void cartesianPlotToQML(std::ostream& ostream);
    //void messageBoxToQML(std::ostream& ostream);
    void meterToQML(std::ostream& ostream);
    //void stripChartToQML(std::ostream& ostream);
    void textUpdateToQML(std::ostream& ostream);

    void choiceButtonToQML(std::ostream& ostream);
    void menuToQML(std::ostream& ostream);
    void messageButtonToQML(std::ostream& ostream);
    void shellCommandToQML(std::ostream& ostream);
    void sliderToQML(std::ostream& ostream);
    void updownButtonToQML(std::ostream& ostream);
    void textEntryToQML(std::ostream& ostream);

    void compositeToQML(std::ostream& ostream);
    void genericToQML(std::ostream& ostream);
    void rectToQML(std::ostream& ostream);

    void exitButtonToQML(std::ostream& ostream);

    virtual void toQML(std::ostream& ostream);

    TOKEN getToken(std::istream &fstream, char *word);
    std::vector<std::string> getLine(std::istream &fstream);
    void parseAndSkip(std::istream &fstream);
    /* access object */
    bool getBool(std::string boolname);
    std::string getColor(std::string colorname);
    std::string getDynamicAttribute(std::string indent);
    std::string getFont(std::string fontname);
    std::vector<std::string> getList(std::string listname);
    std::string getText(std::string textname);
protected:
    Screen *_screen;
    Object *_parent;
    ObjectType _type;
    std::list<Object *> objects;
    int _level;
    int _version;
    Rect _rect;
    std::map<std::string, std::vector<std::string> > properties;
};

class Screen : public Object
{
public:
    Screen();

    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);
    void toPartialQML(std::ostream &ostream);

    virtual ~Screen() {
        for (std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
            delete *it;
    }

    //std::string color(int index) {return colormap.value(index); }

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
    void parseScreen(std::istream &fstream);

private:
    std::string nameValueString;
    std::map<std::string, std::string> nameValueTable;
    std::set<std::string> unmatchedNames;

    /* file */
    std::string file;

    /* display */
    int clr, bclr;

    std::string cmap;
    //ColorMap colormap;
};
