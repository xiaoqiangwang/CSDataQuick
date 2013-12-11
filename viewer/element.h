#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "common.h"

/* Widget Common Attributes */
class Attribute
{
public:
    Attribute () {}
    virtual void parse(istream& fstream) = 0;
    virtual string write() = 0;
};

class BasicAttribute : public Attribute
{
public:
    BasicAttribute();
    void parse(istream &fstream);

private:
    int clr;
    EdgeStyle style;
    FillStyle fill;
    unsigned int width;
};

class DynamicAttribute : public Attribute
{
public:
    DynamicAttribute();
    void parse(istream &fstream);

protected:
    void parseMode(istream &fstream);
    void parseParam(istream &fstream);

private:
    ColorMode clr;
    VisibilityMode vis;
    string chan[MAX_CALC_RECORDS];
    string calc;
    string post;
    bool validCalc;
};

class Object : public Attribute
{
public:
    Object();
    void parse(istream &fstream);

private:
    int x, y;
    unsigned int width, height;
};

class Monitor : public Attribute
{
public:
    Monitor();
private:
    string rdbk;
    int clr, bclr;
};

class Control : public Attribute
{
public:
    Control();
private:
    string control;
    int clr, bclr;
};

class Limits : public Attribute
{
public:
    Limits();
private:
    PvLimitsSrc_t lopr;
    double loprDefault;
    PvLimitsSrc_t hopr;
    double hoprDefault;
    PvLimitsSrc_t prec;
    short precDefault;
};

class Plotcom : public Attribute
{
public:
    Plotcom();
private:
    string title;
    string xlablel;
    string ylabel;
    int clr, bclr;
    string package;
};

class PlotAxisDefinition : public Attribute
{
public:
    PlotAxisDefinition();
private:
    CartesianPlotAxisStyle axisStyle;
    CartesianPlotRangeStyle rangeStyle;
    CartesianPlotTimeFormat_t timeFormat;
    double minimum, maximum;
};

class RelatedDisplayEntry : public Attribute
{
public:
    RelatedDisplayEntry();
private:
    string label;
    string name;
    string args;
    relatedDisplayMode_t mode;
};

class ShellCommandEntry : public Attribute
{
public:
    ShellCommandEntry();
private:
    string label;
    string command;
    string args;
};

/*
 * Top level
 */

class DisplayInfo
{
public:
    DisplayInfo();

    void parse(istream &fstream);
private:
    /* file */
    string file;
    int version;
    /* display */
    //Object object;
    int clr, bclr;
    string cmap;
    vector<unsigned int> colormap;
    /* grid */
    int gridSpacing;
    bool gridOn;
    bool snapToGrid;
    /* macr,value */
    map<string, string> nameValueTable;
};

/* Widget Types */
class Element {
public:
    Element () {
        this->_type = DL_Element;
    }

    DlElementType type() {return _type;}

    virtual bool parse(istream& fstream) = 0;
    virtual string write() = 0;

protected:
    DlElementType _type;
};

class Arc : public Element {
public:
    Arc () {
        this->_type = DL_Arc;
    }
};

#endif // ELEMENT_H
