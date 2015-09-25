#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>

#include "common.h"

/*
 * types for parsing
 */
typedef enum
{ T_WORD, T_EQUAL, T_QUOTE, T_LEFT_BRACE, T_RIGHT_BRACE, T_EOF} TOKEN;

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

class Element;

/* Widget Common Attributes */
class Attribute
{
public:
    Attribute(Element *parent) {_parent = parent;}
    virtual void parse(std::istream& fstream) = 0;
    virtual void toQML(std::ostream& fstream) = 0;
    virtual ~Attribute() {
    }

    Element *parent() { return _parent;}

protected:
    TOKEN getToken(std::istream &fstream, char *word);
    /* to which Element this attribute is belonging to */
    Element *_parent;
};

class ColorMap : public Attribute
{
public:
    ColorMap(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream) {(void)fstream;}

    std::string value(int index) {return colormap[index];}

protected:
    void parseColors(std::istream &fstream);
private:
    int ncolors;
    std::vector<std::string> colormap;
};

class BasicAttribute : public Attribute
{
public:
    BasicAttribute(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

    unsigned int lineWidth() {return width;}
    void setLineWidth(unsigned int lineWidth) {this->width = lineWidth;}

private:
    int clr;
    EdgeStyle style;
    FillStyle fill;
    unsigned int width;
};

class DynamicAttribute : public Attribute
{
public:
    DynamicAttribute(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

    bool hasChannels() {return !chan[0].empty();}

protected:
    ColorMode clr;
    VisibilityMode vis;
    std::string chan[MAX_CALC_RECORDS];
    std::string calc;
    std::string post;
    bool validCalc;
};

class Monitor : public Attribute
{
public:
    Monitor(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

private:
    std::string rdbk;
    int clr, bclr;
};

class Control : public Attribute
{
public:
    Control(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

private:
    std::string control;
    int clr, bclr;
};

class Limits : public Attribute
{
public:
    Limits(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

//private:
    PvLimitsSrc_t loprSrc;
    double loprDefault;
    PvLimitsSrc_t hoprSrc;
    double hoprDefault;
    PvLimitsSrc_t precSrc;
    short precDefault;
};

class Plotcom : public Attribute
{
public:
    Plotcom(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

private:
    std::string title;
    std::string xlabel;
    std::string ylabel;
    int clr, bclr;
    std::string package;
};

class PlotAxisDefinition : public Attribute
{
public:
    PlotAxisDefinition(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

    void setName(std::string name) {this->name = name;}

private:
    std::string name;
    CartesianPlotAxisStyle axisStyle;
    CartesianPlotRangeStyle rangeStyle;
    CartesianPlotTimeFormat_t timeFormat;
    double minimum, maximum;
};

class RelatedDisplayEntry : public Attribute
{
public:
    RelatedDisplayEntry(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

private:
    std::string label;
    std::string name;
    std::string args;
    relatedDisplayMode_t mode;
};

class ShellCommandEntry : public Attribute
{
public:
    ShellCommandEntry(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

private:
    std::string label;
    std::string command;
    std::string args;
};

class Pen : public Attribute
{
public:
    Pen(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

private:
    std::string chan;
    int clr;
    Limits limits;
};

class Trace : public Attribute
{
public:
    Trace(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &fstream);

private:
    std::string xdata;
    std::string ydata;
    int clr;
    int yaxis;
};

/*
 * Top level
 */

/* Widget Base Types */
class Display;
class Element
{
public:
    Element(Element *parent) {
        _level = 0;
        _parent = parent;
        _type = DL_Element;
        /* find out the depth in the element tree */
        Element *p = parent;
        if (p == 0) _display = (Display *)this;
        while (p) {
            _level ++;
            if (p->parent() == 0)
                _display = (Display *)p;
            p = p->parent();
        }
    }
    virtual ~Element() {}

    DlElementType type() {return _type;}
    Element *parent() {return _parent;}
    Display *display() {return _display;}

    int level() {return _level;}

    Rect rect() {return _rect; }
    void setRect(Rect rect) { this->_rect = rect;}

    virtual void parse(std::istream& fstream) = 0;
    void parseObject(std::istream& fstream);
    virtual void toQML(std::ostream& ostream) = 0;

protected:
    TOKEN getToken(std::istream &fstream, char *word);
    void parseAndSkip(std::istream &fstream);
    /* access object */
protected:
    Display *_display;
    Element *_parent;
    DlElementType _type;
    int _level;
    Rect _rect;

friend class Attribute;
};

class Display : public Element
{
public:
    Display(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);
    void toPartialQML(std::ostream &ostream);

    virtual ~Display() {
        for (std::list<Element*>::iterator it = widgets.begin(); it != widgets.end(); ++it)
            delete *it;
    }

    std::string color(int index) {return colormap.value(index); }

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

protected:
    void parseFile(std::istream &fstream);
    void parseDisplay(std::istream &fstream);

private:
    std::string nameValueString;
    std::map<std::string, std::string> nameValueTable;
    std::list<Element *> widgets;

    /* file */
    std::string file;
    int version;

    /* display */
    int clr, bclr;

    std::string cmap;
    ColorMap colormap;

    int gridSpacing;
    bool gridOn;
    bool snapToGrid;
};

class Composite : public Element
{
public:
    Composite(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

    virtual ~Composite() {
        for (std::list<Element*>::iterator it = widgets.begin(); it != widgets.end(); ++it)
            delete *it;
    }

protected:
    void parseChildren(std::istream &fstream);
    void parseCompositeFile(const char *filename);
    void moveChildren(int offsetX, int offsetY);

private:
    DynamicAttribute dynamic_attr;
    std::string name;
    std::string file;
    std::string macro;
    std::list<Element *> widgets;

    friend class Display;
};

/* Graphic Widgets */
class Arc : public Element
{
public:
    Arc (Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    double begin;
    double path;
};

class Image : public Element
{
public:
    Image(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    std::string imageName;
    std::string calc;
};


class Oval : public Element
{
public:
    Oval(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
};

class Polygon : public Element
{
public:
    Polygon(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

protected:
    void parsePoints(std::istream &fstream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    std::vector<Point> points;
};

class Polyline : public Element
{
public:
    Polyline(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

protected:
    void parsePoints(std::istream &fstream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    std::vector<Point> points;
};

class Rectangle : public Element
{
public:
    Rectangle(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
};

class Text : public Element
{
public:
    Text(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    TextAlign align;
    std::string label;
};

/* Control Widegts */
class ChoiceButton : public Element
{
public:
    ChoiceButton(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

private:
    Control control;
    ColorMode clrmod;
    Stacking stacking;
};

class Menu : public Element
{
public:
    Menu(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

private:
    Control control;
    ColorMode clrmod;
};

class MessageButton : public Element
{
public:
    MessageButton(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

private:
    Control control;
    ColorMode clrmod;
    std::string onMessage;
    std::string offMessage;
    std::string label;
};

class ShellCommand : public Element {
public:
    ShellCommand(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

    virtual ~ShellCommand() {
        for (std::vector<ShellCommandEntry*>::iterator it = commands.begin(); it != commands.end(); ++it)
            delete *it;
    }

protected:
    void parseCommand(std::istream);

private:
    int clr;
    int bclr;
    std::string label;
    std::vector<ShellCommandEntry*> commands;
};

class Slider : public Element
{
public:
    Slider(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

private:
    Control control;
    Limits limits;
    ColorMode clrmod;
    Direction direction;
    LabelType label;
    double dPrecision;
};

class TextEntry : public Element
{
public:
    TextEntry(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

private:
    Control control;
    Limits limits;
    TextFormat format;
    ColorMode clrmod;
};

class WheelSwitch : public Element
{
public:
    WheelSwitch(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);

private:
    Control control;
    Limits limits;
    ColorMode clrmod;
    std::string format;
};

/* Monitor Widgets */
class Bar : public Element {
public:
    Bar (Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

private:
    Monitor monitor;
    Limits limits;
    ColorMode clrmod;

    LabelType label;
    Direction direction;
    FillMode fillmod;
};

class Byte : public Element {
public:
    Byte (Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

private:
    Monitor monitor;
    Limits limits;
    ColorMode clrmod;

    LabelType label;
    Direction direction;
    int sbit;
    int ebit;
};

class CartesianPlot : public Element {
public:
    CartesianPlot (Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

    virtual ~CartesianPlot() {
        for (std::vector<Trace*>::iterator it = traces.begin(); it != traces.end(); ++it)
            delete *it;
    }

private:
    Plotcom plotcom;
    PlotAxisDefinition x;
    PlotAxisDefinition y;
    PlotAxisDefinition y2;
    std::vector<Trace*> traces;

    std::string countPvName;
    int count;
    CartesianPlotStyle style;
    EraseOldest erase_oldest;
    std::string trigger;
    std::string erase;
    eraseMode_t eraseMode;
};

class Indicator : public Element {
public:
    Indicator (Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

private:
    Monitor monitor;
    Limits limits;
    ColorMode clrmod;

    Direction direction;
    LabelType label;
};

class Meter : public Element {
public:
    Meter (Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

private:
    Monitor monitor;
    Limits limits;
    ColorMode clrmod;

    LabelType label;
};

class StripChart : public Element {
public:
    StripChart (Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

    virtual ~StripChart() {
        for (std::vector<Pen*>::iterator it = pens.begin(); it != pens.end(); ++it)
            delete *it;
    }

private:
    Plotcom plotcom;
    double period;
    int delay;
    TimeUnits units;
    std::vector<Pen*> pens;
};

class TextUpdate : public Element {
public:
    TextUpdate (Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

private:
    Monitor monitor;
    Limits limits;
    TextFormat format;
    TextAlign align;
    ColorMode clrmod;
};

/* Misc Widgets */
class RelatedDisplay : public Element {
public:
    RelatedDisplay(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);

    virtual ~RelatedDisplay() {
        for (std::vector<RelatedDisplayEntry*>::iterator it = entries.begin(); it != entries.end(); ++it)
            delete *it;
    }
protected:
    void parseEntry(std::istream);

private:
    int clr;
    int bclr;
    std::string label;
    relatedDisplayVisual_t visual;
    std::vector<RelatedDisplayEntry*> entries;
};

#endif // ELEMENT_H
