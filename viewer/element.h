#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>

#include "common.h"

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

class ColorMap
{
public:
    ColorMap();
    void parse(std::istream &fstream);
    void dump();

    std::string value(int index) {return colormap[index];}

protected:
    void parseColors(std::istream &fstream);
private:
    int ncolors;
    std::vector<std::string> colormap;
};

class Element;

/* Widget Common Attributes */
class Attribute
{
public:
    Attribute(Element *parent) {_parent = parent;}
    virtual void parse(std::istream& fstream) = 0;
    virtual void toQML(std::ostream& fstream) = 0;

    Element *parent() { return _parent;}

protected:
    /* to which Element this attribute is belonging to */
    Element *_parent;
};

class BasicAttribute : public Attribute
{
public:
    BasicAttribute(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);
    void dump();
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

private:
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
    void dump();

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
    void dump();
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
    void dump();

private:
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
private:
    std::string title;
    std::string xlablel;
    std::string ylabel;
    int clr, bclr;
    std::string package;
};

class PlotAxisDefinition : public Attribute
{
public:
    PlotAxisDefinition(Element *parent);
private:
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
    void dump();

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
private:
    std::string label;
    std::string command;
    std::string args;
};

class Pen : public Attribute
{
public:
    Pen(Element *parent);
private:
    std::string chan;
    int clr;
    Limits limits;
};

class Trace : public Attribute
{
public:
    Trace(Element *parent);
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
    virtual ~Element() {};

    DlElementType type() {return _type;}
    Element *parent() {return _parent;}
    Display *display() {return _display;}

    int level() {return _level;}

    Rect rect() {return _rect; }
    void setRect(Rect rect) { this->_rect = rect;}

    virtual void parse(std::istream& fstream) = 0;
    void parseObject(std::istream& fstream);
    virtual void toQML(std::ostream& ostream) = 0;
    virtual void dump() =0;

    /* access object */
protected:
    Display *_display;
    Element *_parent;
    DlElementType _type;
    int _level;
    Rect _rect;
};

class Display : public Element
{
public:
    Display(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);
    void dump();

    std::string color(int index) {return colormap.value(index); }

    void setFileName(std::string filename) { file = filename;}
    std::string fileName() { return file; }

    void setMacros(std::map<std::string, std::string> macros) {nameValueTable = macros;}
    std::map<std::string, std::string> macros() {return nameValueTable;}

protected:
    void parseFile(std::istream &fstream);
    void parseDisplay(std::istream &fstream);

private:
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
    void dump();
    void toQML(std::ostream &ostream);

protected:
    void parseChildren(std::istream &fstream);
    void parseCompositeFile(const char *filename);
    void moveChildren(int offsetX, int offsetY);

private:
    DynamicAttribute dynamic_attr;
    std::string name;
    std::string file;
    std::list<Element *> widgets;
};

/* Graphic Widgets */

class Rectangle : public Element
{
public:
    Rectangle(Element *parent);

    void parse(std::istream &fstream);
    void dump();
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
};

class Oval : public Element
{
public:
    Oval(Element *parent);

    void parse(std::istream &fstream);
    void dump();
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
};

class Arc : public Element
{
public:
    Arc (Element *parent);

    void parse(std::istream &fstream);
    void dump();
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    int begin;
    int path;
};

class Polyline : public Element
{
public:
    Polyline(Element *parent);

    void parse(std::istream &fstream);
    void dump();
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    std::vector<Point> points;
};

class Polygon : public Element
{
public:
    Polygon(Element *parent);

    void parse(std::istream &fstream);
    void dump();
    void toQML(std::ostream &ostream);

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    std::vector<Point> points;
};

class Text : public Element
{
public:
    Text(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);
    void dump();

private:
    BasicAttribute basic_attr;
    DynamicAttribute dynamic_attr;
    TextAlign align;
    std::string label;
};

/* Control Widegts */
class TextEntry : public Element
{
public:
    TextEntry(Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);
    void dump();

private:
    Control control;
    Limits limits;
    TextFormat format;
    ColorMode clrmod;
};

class Menu : public Element
{
public:
    Menu(Element *parent);

    void parse(std::istream &fstream);
    void toQML(std::ostream &ostream);
    void dump();

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
    void dump();

private:
    Control control;
    ColorMode clrmod;
    std::string onMessage;
    std::string offMessage;
    std::string label;
};

/* Monitor Widgets */
class TextUpdate : public Element {
public:
    TextUpdate (Element *parent);
    void parse(std::istream &fstream);
    void toQML(std::ostream& ostream);
    void dump();

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
    void dump();

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
