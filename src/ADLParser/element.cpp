/*****************************************************************************\
 * Copyright 2012-2015 Paul Scherrer Institut, Villigen, Switzerland
 *
 * Note: The ADL file parsing, notably the getToken and parse methods, is
 * derived from MEDM. The copyright belongs to the original authors.
\*****************************************************************************/
#include "element.h"

#include <cstdlib>
#include <climits>
#include <cstring>
#include <fstream>
#include <algorithm>


class Element;
template <typename T>
Element* create_element (Element *parent) {
    return (Element *)new T(parent);
}

typedef Element *(*CreateElement)(Element*);
struct ElementFunction {
    const char* name;
    CreateElement function;
} ElementTable [] = {
/* graphics */
    {"arc", create_element<Arc>},
    {"image", create_element<Image>},
    {"oval", create_element<Oval>},
    {"polygon", create_element<Polygon>},
    {"polyline", create_element<Polyline>},
    {"rectangle", create_element<Rectangle>},
    {"text", create_element<Text>},
/* monitor */
    {"bar", create_element<Bar>},
    {"byte", create_element<Byte>},
    {"cartesian plot", create_element<CartesianPlot>},
    {"indicator", create_element<Indicator>},
    {"meter", create_element<Meter>},
    {"strip chart", create_element<StripChart>},
    {"text update", create_element<TextUpdate>},
/* control*/
    {"choice button", create_element<ChoiceButton>},
    {"menu", create_element<Menu>},
    {"message button", create_element<MessageButton>},
    {"shell command", create_element<ShellCommand>},
    {"valuator", create_element<Slider>},
    {"text entry", create_element<TextEntry>},
    {"wheel switch", create_element<WheelSwitch>},
/* misc */
    {"composite", create_element<Composite>},
    {"related display", create_element<RelatedDisplay>}
};
#define NUM_ELEMENTS sizeof(ElementTable) / sizeof(ElementTable[0])

TOKEN Attribute :: getToken(std::istream &fstream, char *word)
{
    return _parent->getToken(fstream, word);
}

BasicAttribute::BasicAttribute (Element *parent)
        : Attribute(parent)
{
    clr = 14;
    style = SOLID;
    fill = F_SOLID;
    width = 0;
}

void BasicAttribute::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(fstream,token)) ) {
        case T_WORD:
            if(!strcmp(token,"clr")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->clr = atoi(token) % DL_MAX_COLORS;
            } else if(!strcmp(token,"style")) {
                getToken(fstream,token);
                getToken(fstream,token);
                if(!strcmp(token,"solid")) {
                    this->style = SOLID;
                } else if(!strcmp(token,"dash")) {
                    this->style = DASH;
                }
            } else if(!strcmp(token,"fill")) {
                getToken(fstream,token);
                getToken(fstream,token);
                if(!strcmp(token,"solid")) {
                    this->fill = F_SOLID;
                } else if(!strcmp(token,"outline")) {
                    this->fill = F_OUTLINE;
                }
           } else if(!strcmp(token,"width")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->width = atoi(token);
            }
        break;
        case T_LEFT_BRACE:
            nestingLevel++;
        break;
        case T_RIGHT_BRACE:
            nestingLevel--;
        break;
        default:
        break;
        }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void BasicAttribute::toQML(std::ostream &ostream)
{
    int indent_level = parent()->level() + 1;
    std::string indent = std::string(indent_level * 4, ' ');

    ostream << indent << "foreground: \"" << parent()->display()->color(this->clr) << '"' << std::endl;
    if (this->width > 0)
        ostream << indent << "lineWidth: " << this->width << std::endl;
    if (this->fill != F_SOLID)
        ostream << indent << "fill: " << qmlValueTable[this->fill] << std::endl;
    if (this->style != SOLID)
        ostream << indent << "edge: " << qmlValueTable[this->style] << std::endl;
}

DynamicAttribute::DynamicAttribute (Element *parent)
        : Attribute(parent)
{
    clr = STATIC;
    vis = V_STATIC;
    validCalc = false;
}

void DynamicAttribute::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"clr")) {
            getToken(fstream,token);
            getToken(fstream,token);
        if(!strcmp(token,"discrete"))
            this->clr = DISCRETE;
        else if(!strcmp(token,"static"))
            this->clr = STATIC;
        else if(!strcmp(token,"alarm"))
            this->clr = ALARM;
        } else if(!strcmp(token,"vis")) {
            getToken(fstream,token);
            getToken(fstream,token);
            if(!strcmp(token,"static"))
                this->vis = V_STATIC;
            else if(!strcmp(token,"if not zero"))
                this->vis = IF_NOT_ZERO;
            else if(!strcmp(token,"if zero"))
                this->vis = IF_ZERO;
            else if(!strcmp(token,"calc"))
                this->vis = V_CALC;
        } else if (!strcmp(token,"calc")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->calc = token;
        } else {
            char chanName[6];
            for(int i=0; i < MAX_CALC_RECORDS; i++) {
                /* Names are chan, chanB, chanC, etc. */
                sprintf(chanName,"chan%c",i?'A'+i:'\0');
                if(!strcmp(token,chanName)) {
                    getToken(fstream,token);
                    getToken(fstream,token);
                    if(strlen(token) > 0) {
                        this->chan[i] = token;
                    }
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void DynamicAttribute::toQML(std::ostream &ostream)
{
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    if (this->clr != STATIC)
        ostream << indent << "colorMode: " << qmlValueTable[this->clr] << std::endl;
    if (this->vis != V_STATIC)
        ostream << indent << "dynamicAttr.visibilityMode: " << qmlValueTable[this->vis] << std::endl;
    if (!this->calc.empty())
        ostream << indent << "dynamicAttr.visibilityCalc: \"" << this->calc << '"' << std::endl;

    for (int i=0; i < MAX_CALC_RECORDS; i++) {
        char chanName[9];
        if (!this->chan[i].empty()) {
            sprintf(chanName,"channel%c",i?'A'+i:'\0');
            ostream << indent << "dynamicAttr." << chanName << ": \"" << this->chan[i] << '"' << std::endl;
        }
    }
 }

Limits::Limits(Element *parent)
    : Attribute(parent)
{
    loprSrc = PV_LIMITS_CHANNEL;
    hoprSrc = PV_LIMITS_CHANNEL;
    precSrc = PV_LIMITS_CHANNEL;

    loprDefault = hoprDefault = precDefault = 0;
}

void Limits::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(fstream,token)) ) {
        case T_WORD:
            if(!strcmp(token,"loprSrc")) {
                getToken(fstream,token);
                getToken(fstream,token);
                for(int i=FIRST_PV_LIMITS_SRC;
                    i < FIRST_PV_LIMITS_SRC+NUM_PV_LIMITS_SRC; i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        this->loprSrc = (PvLimitsSrc_t)i;
                        break;
                    }
                }
            } else if(!strcmp(token,"loprDefault")) {
                getToken(fstream, token);
                getToken(fstream, token);
                this->loprDefault = atof(token);
            } else if(!strcmp(token,"hoprSrc")) {
                getToken(fstream,token);
                getToken(fstream,token);
                for(int i=FIRST_PV_LIMITS_SRC;
                    i < FIRST_PV_LIMITS_SRC+NUM_PV_LIMITS_SRC; i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        this->hoprSrc = (PvLimitsSrc_t)i;
                        break;
                    }
                }
            } else if(!strcmp(token,"hoprDefault")) {
                getToken(fstream, token);
                getToken(fstream, token);
                this->hoprDefault = atof(token);
            } else if(!strcmp(token,"precSrc")) {
                getToken(fstream,token);
                getToken(fstream,token);
                for(int i=FIRST_PV_LIMITS_SRC;
                    i < FIRST_PV_LIMITS_SRC+NUM_PV_LIMITS_SRC; i++) {
                    if(!strcmp(token,stringValueTable[i])) {
                        this->precSrc = (PvLimitsSrc_t)i;
                        break;
                    }
                }
            } else if(!strcmp(token,"precDefault")) {
                getToken(fstream, token);
                getToken(fstream, token);
                this->precDefault = atof(token);
            }
            break;
        case T_LEFT_BRACE:
            nestingLevel++;
        break;
        case T_RIGHT_BRACE:
            nestingLevel--;
        break;
        default:
        break;
        }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void Limits::toQML(std::ostream &fstream)
{
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    if (this->loprSrc != PV_LIMITS_CHANNEL)
    fstream << indent << "limits.loprSrc: " << qmlValueTable[this->loprSrc] << "\n";
    if (this->hoprSrc != PV_LIMITS_CHANNEL)
    fstream << indent << "limits.hoprSrc: " << qmlValueTable[this->hoprSrc] << "\n";
    if (this->precSrc != PV_LIMITS_CHANNEL)
    fstream << indent << "limits.precSrc: " << qmlValueTable[this->precSrc] << "\n";

    if (this->loprDefault != 0)
    fstream << indent << "limits.loprDefault: " << this->loprDefault << "\n";
    if (this->hoprDefault != 0)
    fstream << indent << "limits.hoprDefault: " << this->hoprDefault << "\n";
    if (this->precDefault != 0)
    fstream << indent << "limits.precDefault: " << this->precDefault << "\n";
}

Plotcom::Plotcom(Element *parent)
    : Attribute(parent)
{
    clr = 14;
    bclr = 3;
}

void Plotcom::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"title")) {
            getToken(fstream, token);
            getToken(fstream, token);
            this->title = token;
        } else if(!strcmp(token,"xlabel")) {
            getToken(fstream, token);
            getToken(fstream, token);
            this->xlabel = token;
        } else if (!strcmp(token,"ylabel")) {
            getToken(fstream, token);
            getToken(fstream, token);
            this->ylabel = token;
        } else if (!strcmp(token,"package")) {
            getToken(fstream, token);
            getToken(fstream, token);
            this->package = token;
        } else if (!strcmp(token,"clr")) {
            getToken(fstream, token);
            getToken(fstream, token);
            this->clr = atoi(token);
        } else if (!strcmp(token,"bclr")) {
            getToken(fstream, token);
            getToken(fstream, token);
            this->bclr = atoi(token);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void Plotcom::toQML(std::ostream &fstream)
{
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    if (!title.empty())
        fstream << indent << "title: \"" << title << "\"" << std::endl;
    if (!xlabel.empty())
        fstream << indent << "xLabel: \"" << xlabel << "\"" << std::endl;
    if (!ylabel.empty())
        fstream << indent << "yLabel: \"" << ylabel << "\"" << std::endl;
    fstream << indent << "foreground: \"" << parent()->display()->color(this->clr) << '"' << std::endl;
    fstream << indent << "background: \"" << parent()->display()->color(this->bclr) << '"' << std::endl;
}

PlotAxisDefinition::PlotAxisDefinition(Element *parent)
    : Attribute(parent)
{
    axisStyle = LINEAR_AXIS;
    rangeStyle = CHANNEL_RANGE;
    minimum = 0.0;
    maximum = 1.0;
    timeFormat = HHMMSS;
}

void PlotAxisDefinition::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"axisStyle")) {
            getToken(fstream, token);
            getToken(fstream, token);
            for(int i=FIRST_CARTESIAN_PLOT_AXIS_STYLE;i<FIRST_CARTESIAN_PLOT_AXIS_STYLE+NUM_CARTESIAN_PLOT_AXIS_STYLES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->axisStyle = (CartesianPlotAxisStyle)i;
                    break;
                }
            }
        } else if(!strcmp(token,"rangeStyle")) {
            getToken(fstream, token);
            getToken(fstream, token);
            for(int i=FIRST_CARTESIAN_PLOT_RANGE_STYLE;i<FIRST_CARTESIAN_PLOT_RANGE_STYLE+NUM_CARTESIAN_PLOT_RANGE_STYLES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->rangeStyle = (CartesianPlotRangeStyle)i;
                    break;
                }
            }
        } else if (!strcmp(token,"timeFormat")) {
            getToken(fstream, token);
            getToken(fstream, token);
            for(int i=FIRST_CP_TIME_FORMAT;i<FIRST_CP_TIME_FORMAT+NUM_CP_TIME_FORMAT;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->timeFormat = (CartesianPlotTimeFormat_t)i;
                    break;
                }
            }
        } else if (!strcmp(token,"minRange")) {
            getToken(fstream, token);
            getToken(fstream, token);
            this->minimum = atof(token);
        } else if (!strcmp(token,"maxRange")) {
            getToken(fstream, token);
            getToken(fstream, token);
            this->maximum = atof(token);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void PlotAxisDefinition::toQML(std::ostream &fstream)
{
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    if(this->rangeStyle !=  CHANNEL_RANGE)
        fstream << indent << this->name << "RangeStyle: " << qmlValueTable[this->rangeStyle] << std::endl;
    if (this->minimum != 0)
        fstream << indent << this->name << "RangeLower: " << this->minimum << std::endl;
    if (this->maximum != 1)
        fstream << indent << this->name << "RangeUpper: " << this->maximum << std::endl;
    if(this->axisStyle != LINEAR_AXIS)
        fstream << indent << this->name << "AxisStyle: " << qmlValueTable[this->axisStyle] << std::endl;
    if(this->timeFormat != HHMMSS)
        fstream << indent << this->name << "TimeFormat: " << qmlValueTable[this->timeFormat] << std::endl;
}

Control::Control(Element *parent)
    : Attribute(parent)
{
    control = "";
    clr = 14;
    bclr = 51;
}

void Control::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(fstream,token)) ) {
        case T_WORD:
            if(!strcmp(token,"clr")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->clr = atoi(token) % DL_MAX_COLORS;
            } else if(!strcmp(token,"bclr")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->bclr = atoi(token) % DL_MAX_COLORS;
            } else if (!strcmp(token,"chan")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->control = token;
            }
        break;
        case T_LEFT_BRACE:
            nestingLevel++;
        break;
        case T_RIGHT_BRACE:
            nestingLevel--;
        break;
        default:
        break;
        }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void Control::toQML(std::ostream &fstream)
{
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    fstream << indent << "channel: " << '"' << this->control << '"' << std::endl;
    fstream << indent << "foreground: \"" << parent()->display()->color(this->clr) << '"' << std::endl;
    fstream << indent << "background: \"" << parent()->display()->color(this->bclr) << '"' << std::endl;
}

Monitor::Monitor(Element *parent)
    : Attribute(parent)
{
    rdbk = "";
    clr = 14;
    bclr = 4;
}

void Monitor::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
        switch( (tokenType=getToken(fstream,token)) ) {
        case T_WORD:
            if(!strcmp(token,"clr")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->clr = atoi(token) % DL_MAX_COLORS;
            } else if(!strcmp(token,"bclr")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->bclr = atoi(token) % DL_MAX_COLORS;
            } else if (!strcmp(token,"chan")) {
                getToken(fstream,token);
                getToken(fstream,token);
                this->rdbk = token;
            }
        break;
        case T_LEFT_BRACE:
            nestingLevel++;
        break;
        case T_RIGHT_BRACE:
            nestingLevel--;
        break;
        default:
        break;
        }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void Monitor::toQML(std::ostream &fstream)
{
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    fstream << indent << "channel: " << '"' << this->rdbk << '"' << std::endl;
    fstream << indent << "foreground: \"" << parent()->display()->color(this->clr) << '"' << std::endl;
    fstream << indent << "background: \"" << parent()->display()->color(this->bclr) << '"' << std::endl;
}

RelatedDisplayEntry::RelatedDisplayEntry(Element *parent)
    : Attribute(parent)
{
    this->mode = ADD_NEW_DISPLAY;
}

void RelatedDisplayEntry::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"label")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->label = token;
        } else if(!strcmp(token,"name")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->name = token;
        } else if(!strcmp(token,"args")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->args = token;
        } else if(!strcmp(token,"policy")) {
        getToken(fstream,token);
        getToken(fstream,token);
        if(!strcmp(token,stringValueTable[REPLACE_DISPLAY]))
            this->mode = REPLACE_DISPLAY;
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void RelatedDisplayEntry::toQML(std::ostream &ostream)
{
    if (this->name.empty())
         return;
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "    ListElement {" << std::endl;
    ostream << indent << "        label: \"" << this->label << '"' << std::endl;
    ostream << indent << "        fname: \"" << this->name << '"' << std::endl;
    ostream << indent << "        args: \"" << this->args << '"' << std::endl;
    if (this->mode == REPLACE_DISPLAY)
    ostream << indent << "        remove: true" << std::endl;
    ostream << indent << "    }" << std::endl;
}

TOKEN Element :: getToken(std::istream &fstream, char *word)
{
    enum {NEUTRAL,INQUOTE,INWORD,INMACRO} state = NEUTRAL, savedState = NEUTRAL;
    char c;
    char *w;
    char *m, macro[MAX_TOKEN_LENGTH];
    int j;

    w = word;
    m = macro;

    while( (c=fstream.get()) != EOF) {
        switch (state) {
        case NEUTRAL:
            switch(c) {
            case '=' : return(T_EQUAL);
            case '{' : return(T_LEFT_BRACE);
            case '}' : return(T_RIGHT_BRACE);
            case '"' : state = INQUOTE;
                break;
            case '$' : c=fstream.get();
                /* only do macro substitution if in execute mode or
           * parsing a composite file */
                if( c == '(' ) {
                    state = INMACRO;
                } else {
                    *w++ = '$';
                    *w++ = c;
                }
                break;
            case ' ' :
            case '\t':
            case '\r':
            case '\n': break;

                /* for constructs of the form (a,b) */
            case '(' :
            case ',' :
            case ')' : *w++ = c; *w = '\0'; return(T_WORD);

            default  : state = INWORD;
                *w++ = c;
                break;
            }
            break;
        case INQUOTE:
            switch(c) {
            case '"' : *w = '\0'; return(T_WORD);
            case '$' : c=fstream.get();
                /* do macro substitution */
                if( c == '(' ) {
                    savedState = INQUOTE;
                    state = INMACRO;
                } else {
                    *w++ = '$';
                    *w++ = c;
                }
                break;
            default  : *w++ = c;
                break;
            }
            break;
        case INMACRO:
            switch(c) {
            case ')' : *m = '\0';
                if (display()->findMacro(macro)) {
                    std::string v = display()->macroValue(macro);
                    strncpy(w,  v.c_str(), v.size());
                    w += v.size();
                }
                else {
                    *w++ = '$';
                    *w++ = '(';
                    for(j = 0; j < (int) strlen(macro); j++) {
                        *w++ = macro[j];
                    }
                    *w++ = ')';
                    display()->addUnmatchedMacro(macro);
                }
                state = savedState;
                m = macro;
                break;

            default  : *m++ = c;
                break;
            }
            break;
        case INWORD:
            switch(c) {
            case ' ' :
            case '\r':
            case '\n':
            case '\t':
            case '=' :
            case '(' :
            case ',' :
            case ')' :
            case '"' : fstream.unget(); *w = '\0'; return(T_WORD);
            default  : *w++ = c;
                break;
            }
            break;
        }
    }
    return(T_EOF);
}

void Element :: parseAndSkip(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

  /* Just read and look for braces until we return to the same level */
    do {
        switch( (tokenType=getToken(fstream,token)) ) {
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
        }
    } while( (tokenType != T_RIGHT_BRACE || nestingLevel > 0) &&
      (nestingLevel > 0) && (tokenType != T_EOF) );
}
void Element::parseObject(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"x")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->_rect.x = atoi(token);
        } else if(!strcmp(token,"y")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->_rect.y = atoi(token);
        } else if(!strcmp(token,"width")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->_rect.width = atoi(token);
        } else if(!strcmp(token,"height")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->_rect.height = atoi(token);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void Element::toQML(std::ostream &ostream)
{
    int indent_level = level() + 1;
    std::string indent(indent_level * 4, ' ');

    int offsetX = 0, offsetY = 0;

    if (parent() && parent()->type() != DL_Display) {
        Rect rc = parent()->rect();
        offsetX = rc.x;
        offsetY = rc.y;
    }

    ostream << indent << "x: " << this->_rect.x - offsetX << std::endl;
    ostream << indent << "y: " << this->_rect.y - offsetY << std::endl;
    ostream << indent << "width: " << this->_rect.width << std::endl;
    ostream << indent << "height: " << this->_rect.height << std::endl;
}

Display::Display(Element *parent)
    : Element(parent), colormap((Element *)this)
{
    this->_type = DL_Display;
}

void Display::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    tokenType = getToken(fstream, token);
    if (tokenType == T_WORD && !strcmp(token, "file"))
        this->parseFile(fstream);
    else
        return;

    tokenType = getToken(fstream, token);
    if (tokenType == T_WORD && !strcmp(token, "display"))
        this->parseDisplay(fstream);
    else
        return;

    tokenType = getToken(fstream, token);
    if (tokenType == T_WORD && !strcmp(token, "color map")) {
        this->colormap.parse(fstream);
    }
    else
        return;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
    {
        bool found = false;
        for (size_t i = 0; i < NUM_ELEMENTS; i++) {
            if (!strcmp(token, ElementTable[i].name)) {
                Element *element = ElementTable[i].function(this);
                element->parse(fstream);
                this->widgets.push_back(element);
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << token << " Not Implemented" << std::endl;
            parseAndSkip(fstream);
        }
    }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while ( tokenType != T_EOF );

    /* load external color map */
    if (strlen(token) != 0) {
        std::fstream file(token, std::fstream::in);
        if (file.is_open())
            this->colormap.parse(file);
    }

}

void Display::parseFile(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"name")) {
        getToken(fstream,token);
        getToken(fstream,token);
        /* do not use file path from the file since it may
         * not be the actual location */
        } else if(!strcmp(token,"version")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->version =atoi(token);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void Display::parseDisplay(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
        this->parseObject(fstream);
        } else if(!strcmp(token,"clr")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->clr = atoi(token);
        } else if(!strcmp(token,"bclr")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->bclr = atoi(token);
        } else if(!strcmp(token,"cmap")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->cmap = token;
        } else if(!strcmp(token,"gridSpacing")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->gridSpacing = atoi(token);
        } else if(!strcmp(token,"gridOn")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->gridOn = atoi(token);
        } else if(!strcmp(token,"snapToGrid")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->snapToGrid = atoi(token);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

ColorMap::ColorMap(Element *parent)
    : Attribute(parent)
{

}

void ColorMap::parseColors(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        this->colormap.push_back(std::string("#") + token);
        getToken(fstream,token);
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void ColorMap::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"ncolors")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->ncolors = atoi(token);
        } else if (!strcmp(token,"colors")) {
        this->parseColors(fstream);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Display::toQML(std::ostream &ostream)
{
    ostream << "import QtQuick 2.0\n";
    ostream << "import QtQuick.Controls 1.0\n";
    ostream << "import PvComponents 1.0\n";
    ostream << "BaseWindow {\n";
    Element::toQML(ostream);
    ostream << "    color: \"" << color(this->bclr) << '"' << std::endl;
    ostream << "    filePath: \"" << this->fileName() << "\"" << std::endl;
    ostream << "    macro: \"" << this->macroString() << "\"" << std::endl;
    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it)
        (*it)->toQML(ostream);

    ostream << "}\n";
}

void Display::toPartialQML(std::ostream &ostream)
{
    /* find out the bounding rect */
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it) {
        Element *element = (*it);
        Rect rc = element->rect();
        minX = MIN(rc.x, minX);
        minY = MIN(rc.y, minY);
        maxX = MAX(rc.x + rc.width, maxX);
        maxY = MAX(rc.y + rc.height, maxY);
    }

    /* enlarge composite if necessary */
    _rect.width = MAX(maxX - minX, _rect.width);
    _rect.height = MAX(maxY - minY, _rect.height);

    /* shift all components accordingly */
    int offsetX = - minX, offsetY =  - minY;
    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it) {
        Element *element = (*it);
        Rect rc = element->rect();
        rc.x = rc.x + offsetX;
        rc.y = rc.y + offsetY;
        element->setRect(rc);

        if (element->type() == DL_Composite)
            ((Composite *) element)->moveChildren(offsetX, offsetY);
    }

    /* output to QML for each component */
    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it)
        (*it)->toQML(ostream);
}

Composite::Composite(Element *parent)
    : Element(parent),
      dynamic_attr(this)
{
    this->_type = DL_Composite;

}

void Composite::moveChildren(int offsetX, int offsetY)
{
    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it) {
        Element *element = (*it);
        Rect rc = element->rect();
        rc.x = rc.x + offsetX;
        rc.y = rc.y + offsetY;
        element->setRect(rc);

        if (element->type() == DL_Composite)
            ((Composite *) element)->moveChildren(offsetX, offsetY);
    }
}

void Composite::parseCompositeFile(const char *filename)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;

    std::ifstream fstream;
    fstream.open(filename);

    tokenType = getToken(fstream, token);
    if (tokenType == T_WORD && !strcmp(token, "file"))
        parseAndSkip(fstream);
    else
        return;

    tokenType = getToken(fstream, token);
    if (tokenType == T_WORD && !strcmp(token, "display"))
        parseAndSkip(fstream);
    else
        return;

    tokenType = getToken(fstream, token);
    if (tokenType == T_WORD && !strcmp(token, "color map")) {
        parseAndSkip(fstream);
    }
    else
        return;

    while ( (tokenType=getToken(fstream,token)) != T_EOF ) {
        bool found = false;
        for (size_t i = 0; i < NUM_ELEMENTS; i++) {
            if (!strcmp(token, ElementTable[i].name)) {
                Element *element = ElementTable[i].function(this);
                element->parse(fstream);
                this->widgets.push_back(element);
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << token << " Not Implemented" << std::endl;
            parseAndSkip(fstream);
        }
    }

    /* find out the bounding rect */
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it) {
        Element *element = (*it);
        Rect rc = element->rect();
        minX = MIN(rc.x, minX);
        minY = MIN(rc.y, minY);
        maxX = MAX(rc.x + rc.width, maxX);
        maxY = MAX(rc.y + rc.height, maxY);
    }

    /* enlarge composite if necessary */
    _rect.width = MAX(maxX - minX, _rect.width);
    _rect.height = MAX(maxY - minY, _rect.height);

    /* shift all components accordingly */
    this->moveChildren(_rect.x - minX, _rect.y - minY);
}

void Composite::parseChildren(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
    {
        bool found = false;
        for (size_t i = 0; i < NUM_ELEMENTS; i++) {
            if (!strcmp(token, ElementTable[i].name)) {
                Element *element = ElementTable[i].function(this);
                element->parse(fstream);
                this->widgets.push_back(element);
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << token << " Not Implemented" << std::endl;
            parseAndSkip(fstream);
        }
    }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Composite::parse(std::istream &fstream)
{
    char filename[MAX_TOKEN_LENGTH];
    char macroString[MAX_TOKEN_LENGTH];
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"dynamic attribute")) {
            this->dynamic_attr.parse(fstream);
        } else if (!strcmp(token,"composite name")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->name = token;
        } else if (!strcmp(token,"composite file")) {
            getToken(fstream,token);
            getToken(fstream,token);
            /* Separate filename and macros from dlComposite->compositeFile  */
            strncpy(filename,token,MAX_TOKEN_LENGTH);
            *macroString = '\0';
            filename[MAX_TOKEN_LENGTH-1]='\0';
            if(filename[0]) {
                /* Is of the form filename;a=xxx,b=yyy,... */
                char *ptr = NULL;
                ptr = strchr(filename, ';');
                if(ptr) {
                    /* End the file name at the ; */
                    *ptr = '\0';
                    /* Copy the remainder to the macroString */
                    strcpy(macroString, ++ptr);
                }
            }
            this->file = filename;
            this->macro = macroString;
            if (this->macro.empty())
                this->macro = display()->macroString();
        } else if (!strcmp(token, "children")) {
            this->parseChildren(fstream);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

UpdateType Composite::updateType()
{
    UpdateType type = dynamic_attr.hasChannels() ? DYNAMIC_GRAPHIC : STATIC_GRAPHIC;

    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it) {
        Element * element = (*it);
        UpdateType t = element->updateType();
        if ( t != STATIC_GRAPHIC && type != t) {
            type = t;
            break;
        }
    }
    return type;
}

void Composite::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaComposite {" << std::endl;
    Element::toQML(ostream);
    this->dynamic_attr.toQML(ostream);

    if (this->file.empty()) {
        for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it) {
            Element * element = (*it);
            element->toQML(ostream);
        }
        // if no dynamic channel is defined and no children other than graphics, this is a static composite item.
        // lower its stacking order not to shadow other control/monitor items
        if (this->updateType() == STATIC_GRAPHIC)
            ostream << indent << "    z: -2" << std::endl;
        else if (this->updateType() == DYNAMIC_GRAPHIC)
            ostream << indent << "    z: -1" << std::endl;
    } else {
        ostream << indent << "    source: \"" << this->file << "\"" << std::endl;
        ostream << indent << "    macro: \"" << this->macro << "\"" << std::endl;
    }
    ostream << indent << "}" << std::endl;
}

/******************************************************
 *                   Graphics
 *
 ******************************************************/
Arc::Arc (Element *parent)
    : Element(parent),
      basic_attr(this),
      dynamic_attr(this)
{
    this->_type = DL_Arc;
}

void Arc::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"basic attribute")) {
            this->basic_attr.parse(fstream);
        } else if (!strcmp(token,"dynamic attribute")) {
            this->dynamic_attr.parse(fstream);
        }  else if (!strcmp(token, "begin")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->begin = atoi(token) / 64.;
        }  else if (!strcmp(token, "path")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->path = atoi(token) / 64.;
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Arc::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaArc {" << std::endl;
    Element::toQML(ostream);
    this->basic_attr.toQML(ostream);
    this->dynamic_attr.toQML(ostream);
    ostream << indent << "    begin: " << this->begin << std::endl;
    ostream << indent << "    span: " << this->path << std::endl;
    // if no dynamic channel is defined, this is a static graphics item.
    // lower its stacking order not to shadow other control/monitor items
    if (this->dynamic_attr.hasChannels()) {
        ostream << indent << "    z: -1" << std::endl;
    } else
        ostream << indent << "    z: -2" << std::endl;
    ostream << indent << "}" << std::endl;
}


Image::Image (Element *parent)
    : Element(parent),
      basic_attr(this),
      dynamic_attr(this)
{
    this->_type = DL_Image;
}

void Image::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"basic attribute")) {
            this->basic_attr.parse(fstream);
        } else if (!strcmp(token,"dynamic attribute")) {
            this->dynamic_attr.parse(fstream);
        }  else if (!strcmp(token, "type")) {
            /* type is not ncessary and will be derived from image file */
            getToken(fstream,token);
            getToken(fstream,token);
        }  else if (!strcmp(token, "image name")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->imageName = token;
        } else if (!strcmp(token, "calc")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->calc = token;
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Image::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaImage {" << std::endl;
    Element::toQML(ostream);
    this->basic_attr.toQML(ostream);
    this->dynamic_attr.toQML(ostream);
    ostream << indent << "    source: \"" << this->imageName << "\"" << std::endl;
    ostream << indent << "    imageCalc: \"" << this->calc << "\"" <<std::endl;
    // if no dynamic channel is defined and image has single frame, this is a static graphics item.
    // lower its stacking order not to shadow other control/monitor items
    ostream << indent << "    z: dynamicAttr.channel || frameCount > 1 ? -1 : -2" << std::endl;
    ostream << indent << "}" << std::endl;
}

Oval::Oval (Element *parent)
    : Element(parent),
      basic_attr(this),
      dynamic_attr(this)
{
    this->_type = DL_Oval;
}

void Oval::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"basic attribute")) {
            this->basic_attr.parse(fstream);
        } else if (!strcmp(token,"dynamic attribute")) {
            this->dynamic_attr.parse(fstream);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Oval::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaOval {" << std::endl;
    Element::toQML(ostream);
    this->basic_attr.toQML(ostream);
    this->dynamic_attr.toQML(ostream);
    // if no dynamic channel is defined, this is a static graphics item.
    // lower its stacking order not to shadow other control/monitor items
    if (this->dynamic_attr.hasChannels()) {
        ostream << indent << "    z: -1" << std::endl;
    } else
        ostream << indent << "    z: -2" << std::endl;
    ostream << indent << "}" << std::endl;
}

Polygon::Polygon (Element *parent)
    : Element(parent),
      basic_attr(this),
      dynamic_attr(this)
{
    this->_type = DL_Polygon;
}

void Polygon::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"basic attribute")) {
            this->basic_attr.parse(fstream);
        } else if (!strcmp(token,"dynamic attribute")) {
            this->dynamic_attr.parse(fstream);
        }  else if (!strcmp(token,"points")) {
            this->parsePoints(fstream);
        }

        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Polygon::parsePoints(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    Point point;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"(")) {
            getToken(fstream,token);
            point.x = atoi(token) - this->_rect.x;
            getToken(fstream,token); // separator
            getToken(fstream,token);
            point.y = atoi(token) - this->_rect.y;
            getToken(fstream,token); // ")"
            this->points.push_back(point);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );

}

void Polygon::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaPolygon {" << std::endl;
    Element::toQML(ostream);
    this->basic_attr.toQML(ostream);
    this->dynamic_attr.toQML(ostream);
    ostream << indent << "    points: [";
    for (std::vector<Point>::iterator it = this->points.begin(); it != this->points.end(); it ++) {
        ostream << "Qt.point(" << (*it).x << "," << (*it).y << "),";
    }
    ostream << "]" << std::endl;
    // if no dynamic channel is defined, this is a static graphics item.
    // lower its stacking order not to shadow other control/monitor items
    if (this->dynamic_attr.hasChannels()) {
        ostream << indent << "    z: -1" << std::endl;
    } else
        ostream << indent << "    z: -2" << std::endl;
    ostream << indent << "}" << std::endl;
}

Polyline::Polyline (Element *parent)
    : Element(parent),
      basic_attr(this),
      dynamic_attr(this)
{
    this->_type = DL_Polyline;
}

void Polyline::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"basic attribute")) {
            this->basic_attr.parse(fstream);
        } else if (!strcmp(token,"dynamic attribute")) {
            this->dynamic_attr.parse(fstream);
        }  else if (!strcmp(token,"points")) {
            this->parsePoints(fstream);
        }

        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );

    /* Fixup zero width/height for straight line */
    if (this->_rect.width == 0)
        this->_rect.width = std::max(unsigned(1), this->basic_attr.lineWidth());
    if (this->_rect.height == 0)
        this->_rect.height = std::max(unsigned(1), this->basic_attr.lineWidth());
}

void Polyline::parsePoints(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    Point point;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"(")) {
            getToken(fstream,token);
            point.x = atoi(token) - this->_rect.x;
            getToken(fstream,token); // separator
            getToken(fstream,token);
            point.y = atoi(token) - this->_rect.y;
            getToken(fstream,token); // ")"
            this->points.push_back(point);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Polyline::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaPolyline {" << std::endl;
    Element::toQML(ostream);
    this->basic_attr.toQML(ostream);
    this->dynamic_attr.toQML(ostream);
    ostream << indent << "    points: [";
    for (std::vector<Point>::iterator it = this->points.begin(); it != this->points.end(); it ++) {
        ostream << "Qt.point(" << (*it).x << "," << (*it).y << "),";
    }
    ostream << "]" << std::endl;
    // if no dynamic channel is defined, this is a static graphics item.
    // lower its stacking order not to shadow other control/monitor items
    if (this->dynamic_attr.hasChannels()) {
        ostream << indent << "    z: -1" << std::endl;
    } else
        ostream << indent << "    z: -2" << std::endl;
    ostream << indent << "}" << std::endl;
}

Rectangle::Rectangle (Element *parent)
    : Element(parent),
      basic_attr(this),
      dynamic_attr(this)
{
    this->_type = DL_Rectangle;
}

void Rectangle::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"basic attribute")) {
            this->basic_attr.parse(fstream);
        } else if (!strcmp(token,"dynamic attribute")) {
            this->dynamic_attr.parse(fstream);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Rectangle::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaRect {" << std::endl;
    Element::toQML(ostream);
    this->basic_attr.toQML(ostream);
    this->dynamic_attr.toQML(ostream);
    // if no dynamic channel is defined, this is a static graphics item.
    // lower its stacking order not to shadow other control/monitor items
    if (this->dynamic_attr.hasChannels()) {
        ostream << indent << "    z: -1" << std::endl;
    } else
        ostream << indent << "    z: -2" << std::endl;
    ostream << indent << "}" << std::endl;
}

Text::Text (Element *parent)
    : Element(parent),
      basic_attr(this),
      dynamic_attr(this)
{
    this->_type = DL_Text;
    this->align = HORIZ_LEFT;
}

void Text::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"basic attribute")) {
            this->basic_attr.parse(fstream);
        } else if (!strcmp(token,"dynamic attribute")) {
            this->dynamic_attr.parse(fstream);
        } else if (!strcmp(token, "textix")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->label = token;
        } else if (!strcmp(token,"align")) {
            getToken(fstream,token);
            getToken(fstream,token);
            int found = 0;
            for(int i=FIRST_TEXT_ALIGN;i<FIRST_TEXT_ALIGN+NUM_TEXT_ALIGNS; i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->align = (TextAlign)i;
                    found=1;
                    break;
                }
            }
            /* Backward compatibility */
            if(!found) {
                if(!strcmp(token,"vert. top")) {
                this->align = HORIZ_LEFT;
                } else if(!strcmp(token,"vert. centered")) {
                this->align = HORIZ_CENTER;
                } else if(!strcmp(token,"vert. bottom")) {
                this->align = HORIZ_RIGHT;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Text::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaText {" << std::endl;
    Element::toQML(ostream);
    this->basic_attr.toQML(ostream);
    this->dynamic_attr.toQML(ostream);
    if (this->align != HORIZ_LEFT)
        ostream << indent << "    align: " << qmlValueTable[this->align] << std::endl;
    ostream << indent << "    text: \"" << this->label << '"' << std::endl;
    // if no dynamic channel is defined, this is a static graphics item.
    // lower its stacking order not to shadow other control/monitor items
    if (this->dynamic_attr.hasChannels()) {
        ostream << indent << "    z: -1" << std::endl;
    } else
        ostream << indent << "    z: -2" << std::endl;
    ostream << indent << "}" << std::endl;
}

/******************************************************
 *                   Controls
 *
 ******************************************************/

ChoiceButton::ChoiceButton (Element *parent)
    : Element(parent),
      control(this)
{
    this->_type = DL_ChoiceButton;
    this->clrmod = STATIC;
    this->stacking = ROW;
}

void ChoiceButton::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"control")) {
            this->control.parse(fstream);
        } else if (!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        } else if (!strcmp(token,"stacking")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_STACKING;i<FIRST_STACKING+NUM_STACKINGS;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->stacking = (Stacking)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void ChoiceButton::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaChoiceButton {" << std::endl;
    Element::toQML(ostream);
    this->control.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (this->stacking != ROW)
        ostream << indent << "    stacking: " << qmlValueTable[this->stacking] << std::endl;
    ostream << indent << "}" << std::endl;
}

Menu::Menu (Element *parent)
    : Element(parent),
      control(this)
{
    this->_type = DL_Menu;
    this->clrmod = STATIC;
}

void Menu::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"control")) {
            this->control.parse(fstream);
        } else if (!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Menu::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaMenu {" << std::endl;
    Element::toQML(ostream);
    this->control.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    ostream << indent << "}" << std::endl;
}

MessageButton::MessageButton (Element *parent)
    : Element(parent),
      control(this)
{
    this->_type = DL_MessageButton;
    this->clrmod = STATIC;
}

void MessageButton::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"control")) {
            this->control.parse(fstream);
        } else if (!strcmp(token,"press_msg")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->onMessage = token;
        } else if (!strcmp(token,"release_msg")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->offMessage = token;
        } else if (!strcmp(token,"label")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->label = token;
        } else if (!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void MessageButton::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaMessageButton {" << std::endl;
    Element::toQML(ostream);
    this->control.toQML(ostream);

    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (!this->onMessage.empty())
        ostream << indent << "    onMessage: \"" << this->onMessage << '"' << std::endl;
    if (!this->offMessage.empty())
        ostream << indent << "    offMessage: \"" << this->offMessage << '"' << std::endl;
    ostream << indent << "    text: \"" << this->label << '"' << std::endl;
    ostream << indent << "}" << std::endl;
}

ShellCommandEntry::ShellCommandEntry(Element *parent)
    : Attribute(parent)
{
}

void ShellCommandEntry::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if(!strcmp(token,"label")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->label = token;
        } else if(!strcmp(token,"name")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->command = token;
        } else if(!strcmp(token,"args")) {
        getToken(fstream,token);
        getToken(fstream,token);
        this->args = token;
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
      && (tokenType != T_EOF) );
}

void ShellCommandEntry::toQML(std::ostream &ostream)
{
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "    ListElement {" << std::endl;
    ostream << indent << "        label: \"" << this->label << '"' << std::endl;
    ostream << indent << "        command: \"" << this->command << '"' << std::endl;
    ostream << indent << "        args: \"" << this->args << '"' << std::endl;
    ostream << indent << "    }" << std::endl;
}

ShellCommand::ShellCommand(Element *parent)
    : Element(parent)
{
    this->_type = DL_ShellCommand;
    clr = 14;
    bclr = 51;
}

void ShellCommand::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"clr")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->clr = atoi(token) % DL_MAX_COLORS;
        } else if (!strcmp(token,"bclr")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->bclr = atoi(token) % DL_MAX_COLORS;
        } else if (!strcmp(token,"label")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->label = token;
        } else if (!strncmp(token,"command",7)) {
            ShellCommandEntry *command = new ShellCommandEntry(this);
            command->parse(fstream);
            this->commands.push_back(command);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void ShellCommand::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaShellCommand {" << std::endl;
    Element::toQML(ostream);
    ostream << indent << "    foreground: \"" << display()->color(this->clr) << '"' << std::endl;
    ostream << indent << "    background: \"" << display()->color(this->bclr) << '"' << std::endl;
    ostream << indent << "    label: \"" << this->label << '"' << std::endl;
    if (commands.size() > 0) {
        ostream << indent << "    model: ListModel {" << std::endl;
        for (std::vector<ShellCommandEntry*>::iterator it=commands.begin(); it != commands.end(); ++it) {
            (*it)->toQML(ostream);
        }
        ostream << indent << "    }" << std::endl;
    }
    ostream << indent << "}" << std::endl;
}

Slider::Slider (Element *parent)
    : Element(parent),
      control(this),
      limits(this)
{
    this->_type = DL_Valuator;
    this->clrmod = STATIC;
    this->label = LABEL_NONE;
    this->direction = RIGHT;
    this->dPrecision = 1.0;
}

void Slider::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"control")) {
            this->control.parse(fstream);
        } else if (!strcmp(token, "limits")) {
            this->limits.parse(fstream);
        } else if (!strcmp(token,"dPrecision")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->dPrecision = atof(token);
        } else if (!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        } else if (!strcmp(token,"label")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_LABEL_TYPE;i<FIRST_LABEL_TYPE+NUM_LABEL_TYPES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->label = (LabelType)i;
                    break;
                }
            }
        } else if (!strcmp(token,"direction")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_DIRECTION;i<FIRST_DIRECTION+NUM_DIRECTIONS;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->direction = (Direction)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Slider::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaSlider {" << std::endl;
    Element::toQML(ostream);
    this->control.toQML(ostream);
    this->limits.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (this->label !=  LABEL_NONE)
        ostream << indent << "    label: " << qmlValueTable[this->label] << std::endl;
    if (this->direction != RIGHT)
        ostream << indent << "    direction: " << qmlValueTable[this->direction] << std::endl;
    if (this->dPrecision != 1.0)
        ostream << indent << "    stepSize: " << this->dPrecision << std::endl;

    ostream << indent << "}" << std::endl;
}

TextEntry::TextEntry (Element *parent)
    : Element(parent),
      control(this),
      limits(this)
{
    this->_type = DL_TextEntry;
    this->format = MEDM_DECIMAL;
    this->clrmod = STATIC;
}

void TextEntry::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"control")) {
            this->control.parse(fstream);
        } else if (!strcmp(token, "limits")) {
            this->limits.parse(fstream);
        } else if (!strcmp(token,"format")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_TEXT_FORMAT;i<FIRST_TEXT_FORMAT+NUM_TEXT_FORMATS; i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->format = (TextFormat)i;
                    break;
                }
            }
        } else if (!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void TextEntry::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaTextEntry {" << std::endl;
    Element::toQML(ostream);
    this->control.toQML(ostream);
    this->limits.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (this->format != MEDM_DECIMAL)
        ostream << indent << "    format: " << qmlValueTable[this->format] << std::endl;
    ostream << indent << "}" << std::endl;
}

WheelSwitch::WheelSwitch (Element *parent)
    : Element(parent),
      control(this),
      limits(this)
{
    this->_type = DL_WheelSwitch;
    this->clrmod = STATIC;
    this->format = "";
}

void WheelSwitch::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"control")) {
            this->control.parse(fstream);
        } else if (!strcmp(token, "limits")) {
            this->limits.parse(fstream);
        } else if (!strcmp(token, "format")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->format = token;
        } else if (!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void WheelSwitch::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaWheelSwitch {" << std::endl;
    Element::toQML(ostream);
    this->control.toQML(ostream);
    this->limits.toQML(ostream);
    if (!this->format.empty())
        ostream << indent << "    format: \"" << this->format << "\"" << std::endl;
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;

    ostream << indent << "}" << std::endl;
}

/******************************************************
 *                   Monitors
 *
 ******************************************************/
Bar::Bar (Element *parent)
    : Element(parent),
      monitor(this),
      limits(this)
{
    this->_type = DL_Bar;
    this->clrmod = STATIC;

    this->label = LABEL_NONE;
    this->direction = RIGHT;
    this->fillmod = FROM_EDGE;
}

void Bar::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"monitor")) {
            this->monitor.parse(fstream);
        } else if (!strcmp(token,"limits")) {
            this->limits.parse(fstream);
        } else if(!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        } else if (!strcmp(token,"label")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_LABEL_TYPE;i<FIRST_LABEL_TYPE+NUM_LABEL_TYPES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->label = (LabelType)i;
                    break;
                }
            }
        } else if (!strcmp(token,"direction")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_DIRECTION;i<FIRST_DIRECTION+NUM_DIRECTIONS;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->direction = (Direction)i;
                    break;
                }
            }
        } else if (!strcmp(token,"fillmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_FILL_MODE;i<FIRST_FILL_MODE+NUM_FILL_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->fillmod = (FillMode)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Bar::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaBar {" << std::endl;
    Element::toQML(ostream);
    this->monitor.toQML(ostream);
    this->limits.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (this->label !=  LABEL_NONE)
        ostream << indent << "    label: " << qmlValueTable[this->label] << std::endl;
    if (this->direction != RIGHT)
        ostream << indent << "    direction: " << qmlValueTable[this->direction] << std::endl;
    if (this->fillmod != FROM_EDGE)
        ostream << indent << "    fillMode: " << qmlValueTable[this->fillmod] << std::endl;
    ostream << indent << "}" << std::endl;
}

Byte::Byte (Element *parent)
    : Element(parent),
      monitor(this),
      limits(this)
{
    this->_type = DL_Byte;
    this->clrmod = STATIC;

    this->label = LABEL_NONE;
    this->direction = RIGHT;
    sbit = 15;
    ebit = 0;
}

void Byte::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"monitor")) {
            this->monitor.parse(fstream);
        } else if (!strcmp(token,"limits")) {
            this->limits.parse(fstream);
        } else if(!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        } else if (!strcmp(token,"direction")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_DIRECTION;i<FIRST_DIRECTION+NUM_DIRECTIONS;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->direction = (Direction)i;
                    break;
                }
            }
        } else if (!strcmp(token,"sbit")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->sbit = atoi(token);
        } else if (!strcmp(token,"ebit")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->ebit = atoi(token);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Byte::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaByte {" << std::endl;
    Element::toQML(ostream);
    this->monitor.toQML(ostream);
    this->limits.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (this->direction != RIGHT)
        ostream << indent << "    direction: " << qmlValueTable[this->direction] << std::endl;
    if (this->sbit != 15)
        ostream << indent << "    start: " << this->sbit << std::endl;
    if (this->ebit != 0)
        ostream << indent << "    end: " << this->ebit << std::endl;
    ostream << indent << "}" << std::endl;
}

CartesianPlot::CartesianPlot (Element *parent)
    : Element(parent),
      plotcom(this),
      x(this),y(this),y2(this)
{
    this->_type = DL_CartesianPlot;

    count = 0;
    style = POINT_PLOT;
    erase_oldest = ERASE_OLDEST_OFF;
    eraseMode = ERASE_IF_NOT_ZERO;
}

void CartesianPlot::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"plotcom")) {
            this->plotcom.parse(fstream);
        } else if(!strcmp(token,"count")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->count = atoi(token);
            this->countPvName = token;
        } else if(!strcmp(token,"countPvName")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->count = atoi(token);
            this->countPvName = token;
        } else if(!strcmp(token,"trigger")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->trigger = token;
        } else if(!strcmp(token,"erase")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->erase = token;
        } else if (!strcmp(token,"eraseMode")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_ERASE_MODE;i<FIRST_ERASE_MODE+NUM_ERASE_MODES; i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->eraseMode = (eraseMode_t)i;
                    break;
                }
            }
        } else if (!strcmp(token,"erase_oldest")) {
            getToken(fstream,token);
            getToken(fstream,token);
            if(!strcmp(token,"on"))
                this->erase_oldest = ERASE_OLDEST_ON;
            else if (!strcmp(token,"off"))
                this->erase_oldest = ERASE_OLDEST_OFF;
            else if (!strcmp(token,"plot last n pts"))
                this->erase_oldest = ERASE_OLDEST_ON;
            else if (!strcmp(token,"plot n pts & stop"))
                this->erase_oldest = ERASE_OLDEST_OFF;
        } else if (!strcmp(token,"style")) {
            getToken(fstream,token);
            getToken(fstream,token);
            if(!strcmp(token,"point plot"))
                this->style = POINT_PLOT;
            else if(!strcmp(token,"point"))
                this->style = POINT_PLOT;
            else if(!strcmp(token,"line plot"))
                this->style = LINE_PLOT;
            else if(!strcmp(token,"line"))
                this->style = LINE_PLOT;
            else if(!strcmp(token,"fill under"))
                this->style = FILL_UNDER_PLOT;
            else if(!strcmp(token,"fill-under"))
                this->style = FILL_UNDER_PLOT;
        } else if (!strcmp(token,"x_axis")) {
            this->x.setName("x");
            this->x.parse(fstream);
        } else if (!strcmp(token,"y1_axis")) {
            this->y.setName("y");
            this->y.parse(fstream);
        } else if (!strcmp(token,"y2_axis")) {
            this->y2.setName("y2");
            this->y2.parse(fstream);
        } else if (!strncmp(token,"trace", 5)) {
            //int traceNumber = MIN(token[6] - '0', MAX_TRACES - 1);
            Trace *trace = new Trace(this);
            trace->parse(fstream);
            this->traces.push_back(trace);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void CartesianPlot::toQML(std::ostream &fstream)
{
    std::string indent(level() * 4, ' ');

    fstream << indent << "CaCartesianPlot {" << std::endl;
    Element::toQML(fstream);
    this->plotcom.toQML(fstream);
    fstream << indent << "    plotStyle: " << qmlValueTable[this->style] << std::endl;
    fstream << indent << "    count: " << this->count << std::endl;
    fstream << indent << "    countPvName: \"" << this->countPvName << "\"" << std::endl;
    fstream << indent << "    triggerPvName: \"" << this->trigger << "\"" << std::endl;
    fstream << indent << "    erasePvName: \"" << this->erase << "\"" << std::endl;
    fstream << indent << "    eraseMode: " << qmlValueTable[this->eraseMode] << std::endl;
    fstream << indent << "    eraseOldest: " << (this->erase_oldest == ERASE_OLDEST_ON ? "true" : "false") << std::endl;

    this->x.toQML(fstream);
    this->y.toQML(fstream);
    this->y2.toQML(fstream);

    if (traces.size() > 0) {
        fstream << indent << "    model: ListModel {" << std::endl;
        for (std::vector<Trace*>::iterator it=traces.begin(); it != traces.end(); ++it) {
            (*it)->toQML(fstream);
        }
        fstream << indent << "    }" << std::endl;
    }

    fstream << indent << "}" << std::endl;
}

Trace::Trace(Element *parent)
    : Attribute(parent)
{

}

void Trace::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"xdata")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->xdata = token;
        } else if (!strcmp(token,"ydata")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->ydata = token;
        } else if (!strcmp(token,"data_clr")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->clr = atoi(token);
        } else if (!strcmp(token,"yaxis")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->yaxis = atoi(token);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Trace::toQML(std::ostream &fstream)
{

    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    fstream << indent << "    ListElement {" << std::endl;
    fstream << indent << "        xchannel: \"" << this->xdata << '"' << std::endl;
    fstream << indent << "        ychannel: \"" << this->ydata << '"' << std::endl;
    fstream << indent << "        foreground: \"" << parent()->display()->color(this->clr) << '"' << std::endl;
    fstream << indent << "        yaxis: " << this->yaxis << std::endl;
    fstream << indent << "    }" << std::endl;
}

Indicator::Indicator (Element *parent)
    : Element(parent),
      monitor(this),
      limits(this)
{
    this->_type = DL_Indicator;

    this->clrmod = STATIC;
    this->label = LABEL_NONE;
    this->direction = RIGHT;
}

void Indicator::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"monitor")) {
            this->monitor.parse(fstream);
        } else if (!strcmp(token,"limits")) {
            this->limits.parse(fstream);
        } else if(!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        } else if (!strcmp(token,"label")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_LABEL_TYPE;i<FIRST_LABEL_TYPE+NUM_LABEL_TYPES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->label = (LabelType)i;
                    break;
                }
            }
        } else if (!strcmp(token,"direction")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_DIRECTION;i<FIRST_DIRECTION+NUM_DIRECTIONS;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->direction = (Direction)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Indicator::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaIndicator {" << std::endl;
    Element::toQML(ostream);
    this->monitor.toQML(ostream);
    this->limits.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (this->label !=  LABEL_NONE)
        ostream << indent << "    label: " << qmlValueTable[this->label] << std::endl;
    if (this->direction != RIGHT)
        ostream << indent << "    direction: " << qmlValueTable[this->direction] << std::endl;
    ostream << indent << "}" << std::endl;
}

Meter::Meter (Element *parent)
    : Element(parent),
      monitor(this),
      limits(this)
{
    this->_type = DL_Meter;

    this->clrmod = STATIC;
    this->label = LABEL_NONE;
}

void Meter::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"monitor")) {
            this->monitor.parse(fstream);
        } else if (!strcmp(token,"limits")) {
            this->limits.parse(fstream);
        } else if(!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        } else if (!strcmp(token,"label")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_LABEL_TYPE;i<FIRST_LABEL_TYPE+NUM_LABEL_TYPES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->label = (LabelType)i;
                    break;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Meter::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaMeter {" << std::endl;
    Element::toQML(ostream);
    this->monitor.toQML(ostream);
    this->limits.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (this->label !=  LABEL_NONE)
        ostream << indent << "    label: " << qmlValueTable[this->label] << std::endl;
    ostream << indent << "}" << std::endl;
}

StripChart::StripChart(Element *parent)
    : Element(parent),
      plotcom(this)
{
    this->_type = DL_StripChart;

    this->delay = -1;
    this->period = 60.0;
    this->units = SECONDS;
}

void StripChart::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"plotcom")) {
            this->plotcom.parse(fstream);
        } else if(!strcmp(token,"period")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->period = atof(token);
        } else if(!strcmp(token,"delay")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->delay = atoi(token);
        } else if (!strcmp(token,"units")) {
            getToken(fstream,token);
            getToken(fstream,token);
            if(!strcmp(token,"minute"))
                this->units = MINUTES;
            else if(!strcmp(token,"second"))
                this->units = SECONDS;
            else if(!strcmp(token,"milli second"))
                this->units= MILLISECONDS;
            else if(!strcmp(token,"milli-second"))
                this->units = MILLISECONDS;
        } else if (!strncmp(token,"pen", 3)) {
            //int penNumber = MIN(token[4] - '0', MAX_PENS - 1);
            Pen *pen = new Pen(this);
            pen->parse(fstream);
            this->pens.push_back(pen);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void StripChart::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaStripChart {" << std::endl;
    Element::toQML(ostream);
    this->plotcom.toQML(ostream);
    ostream << indent << "    period: " << this->period << std::endl;
    if (this->units != SECONDS)
        ostream << indent << "    units: " << qmlValueTable[this->units] << std::endl;
    if (pens.size() > 0) {
        ostream << indent << "    model: ListModel {" << std::endl;
        for (std::vector<Pen*>::iterator it=pens.begin(); it != pens.end(); ++it) {
            (*it)->toQML(ostream);
        }
        ostream << indent << "    }" << std::endl;
    }
    ostream << indent << "}" << std::endl;
}

Pen::Pen(Element *parent)
    : Attribute(parent),
      limits(parent)
{
    this->clr = 14;
}

void Pen::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"chan")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->chan = token;
        } else if (!strcmp(token,"clr")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->clr = atoi(token);
        } else if (!strcmp(token,"limits")) {
            this->limits.parse(fstream);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void Pen::toQML(std::ostream &fstream)
{
    int indent_level = parent()->level() + 1;
    std::string indent(indent_level * 4, ' ');

    fstream << indent << "    ListElement {" << std::endl;
    fstream << indent << "        channel: \"" << this->chan << '"' << std::endl;
    fstream << indent << "        foreground: \"" << parent()->display()->color(this->clr) << '"' << std::endl;
    // Since ListElement can only contain plain types, not QtObject,
    // the limits property has to be given explicitly.
    // This requires acess Limits' members.
    //this->limits.toQML(fstream);
    if (this->limits.loprSrc != PV_LIMITS_CHANNEL)
    fstream << indent << "    loprSrc: " << qmlValueTable[this->limits.loprSrc] << "\n";
    if (this->limits.hoprSrc != PV_LIMITS_CHANNEL)
    fstream << indent << "    hoprSrc: " << qmlValueTable[this->limits.hoprSrc] << "\n";

    if (this->limits.loprDefault != 0)
    fstream << indent << "        loprDefault: " << this->limits.loprDefault << "\n";
    if (this->limits.hoprDefault != 0)
    fstream << indent << "        hoprDefault: " << this->limits.hoprDefault << "\n";

    fstream << indent << "    }" << std::endl;
}

TextUpdate::TextUpdate (Element *parent)
    : Element(parent),
      monitor(this),
      limits(this)
{
    this->_type = DL_TextUpdate;
    this->format = MEDM_DECIMAL;
    this->clrmod = STATIC;
    this->align = HORIZ_LEFT;
}

void TextUpdate::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"monitor")) {
            this->monitor.parse(fstream);
        } else if (!strcmp(token,"limits")) {
            this->limits.parse(fstream);
        } else if(!strcmp(token,"clrmod")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_COLOR_MODE;i<FIRST_COLOR_MODE+NUM_COLOR_MODES;i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->clrmod = (ColorMode)i;
                    break;
                }
            }
        } else if (!strcmp(token,"format")) {
            getToken(fstream,token);
            getToken(fstream,token);
            int found = 0;
            for(int i=FIRST_TEXT_FORMAT;i<FIRST_TEXT_FORMAT+NUM_TEXT_FORMATS; i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->format = (TextFormat)i;
                    found = 1;
                    break;
                }
            }
            /* Backward compatibility */
            if(!found) {
                if(!strcmp(token,"decimal")) {
                this->format = MEDM_DECIMAL;
                } else if(!strcmp(token,
                  "decimal- exponential notation")) {
                this->format = EXPONENTIAL;
                } else if(!strcmp(token,"engr. notation")) {
                this->format = ENGR_NOTATION;
                } else if(!strcmp(token,"decimal- compact")) {
                this->format = COMPACT;
                } else if(!strcmp(token,"decimal- truncated")) {
                this->format = TRUNCATED;
                  /* (MDA) allow for LANL spelling errors {like
                             above, but with trailing space} */
                } else if(!strcmp(token,"decimal- truncated ")) {
                this->format = TRUNCATED;
                  /* (MDA) allow for LANL spelling errors
                             {hexidecimal vs. hexadecimal} */
                } else if(!strcmp(token,"hexidecimal")) {
                this->format = HEXADECIMAL;
                }
            }
        } else if (!strcmp(token,"align")) {
            getToken(fstream,token);
            getToken(fstream,token);
            int found = 0;
            for(int i=FIRST_TEXT_ALIGN;i<FIRST_TEXT_ALIGN+NUM_TEXT_ALIGNS; i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->align = (TextAlign)i;
                    found=1;
                    break;
                }
            }
            /* Backward compatibility */
            if(!found) {
                if(!strcmp(token,"vert. top")) {
                this->align = HORIZ_LEFT;
                } else if(!strcmp(token,"vert. centered")) {
                this->align = HORIZ_CENTER;
                } else if(!strcmp(token,"vert. bottom")) {
                this->align = HORIZ_RIGHT;
                }
            }
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void TextUpdate::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaTextUpdate {" << std::endl;
    Element::toQML(ostream);
    this->monitor.toQML(ostream);
    this->limits.toQML(ostream);
    if (this->clrmod != STATIC)
        ostream << indent << "    colorMode: " << qmlValueTable[this->clrmod] << std::endl;
    if (this->format != MEDM_DECIMAL)
        ostream << indent << "    format: " << qmlValueTable[this->format] << std::endl;
    if (this->align != HORIZ_LEFT)
        ostream << indent << "    align: " << qmlValueTable[this->align] << std::endl;
    ostream << indent << "}" << std::endl;
}

RelatedDisplay::RelatedDisplay(Element *parent)
    : Element(parent)
{
    this->_type = DL_RelatedDisplay;
    clr = 14;
    bclr = 51;
    visual = RD_MENU;
}

void RelatedDisplay::parse(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        if (!strcmp(token,"object")) {
            this->parseObject(fstream);
        } else if (!strcmp(token,"clr")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->clr = atoi(token) % DL_MAX_COLORS;
        } else if (!strcmp(token,"bclr")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->bclr = atoi(token) % DL_MAX_COLORS;
        } else if (!strcmp(token,"label")) {
            getToken(fstream,token);
            getToken(fstream,token);
            this->label = token;
        } else if (!strcmp(token,"visual")) {
            getToken(fstream,token);
            getToken(fstream,token);
            for(int i=FIRST_RD_VISUAL;i<FIRST_RD_VISUAL+NUM_RD_VISUAL; i++) {
                if(!strcmp(token,stringValueTable[i])) {
                    this->visual = (relatedDisplayVisual_t)i;
                    break;
                }
            }
        } else if (!strncmp(token,"display",7)) {
            RelatedDisplayEntry *entry = new RelatedDisplayEntry(this);
            entry->parse(fstream);
            this->entries.push_back(entry);
        }
        break;
    case T_LEFT_BRACE:
        nestingLevel++;
        break;
    case T_RIGHT_BRACE:
        nestingLevel--;
        break;
    default:
        break;
    }
    } while( (tokenType != T_RIGHT_BRACE) && (nestingLevel > 0)
             && (tokenType != T_EOF) );
}

void RelatedDisplay::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaRelatedDisplay {" << std::endl;
    Element::toQML(ostream);
    ostream << indent << "    foreground: \"" << display()->color(this->clr) << '"' << std::endl;
    ostream << indent << "    background: \"" << display()->color(this->bclr) << '"' << std::endl;
    ostream << indent << "    label: \"" << this->label << '"' << std::endl;
    if (this->visual != RD_MENU)
        ostream << indent << "    visual: " << qmlValueTable[this->visual] << std::endl;
    if (entries.size() > 0) {
        ostream << indent << "    model: ListModel {" << std::endl;
        for (std::vector<RelatedDisplayEntry*>::iterator it=entries.begin(); it != entries.end(); ++it) {
            (*it)->toQML(ostream);
        }
        ostream << indent << "    }" << std::endl;
    }
    ostream << indent << "}" << std::endl;
}
