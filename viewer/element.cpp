#include "element.h"
#include "fileinfo.h"
#include <cstdlib>
#include <climits>
#include <cstring>
#include <fstream>

/*******************************************************************************
 * Common Attributes
 *
 */

class Element;
template <typename T>
Element* create_element (Element *parent) {
    return (Element *)new T(parent);
}

typedef Element *(*CreateElement)(Element*);
struct {
    const char* name;
    CreateElement function;
} ElementTable [] = {
/* graphics */
{"rectangle", create_element<Rectangle>},
{"text", create_element<Text>},
/* monitor */
{"text update", create_element<TextUpdate>},
/* control*/
{"text entry", create_element<TextEntry>},
{"menu", create_element<Menu>},
{"message button", create_element<MessageButton>},
/* misc */
{"composite", create_element<Composite>},
{"related display", create_element<RelatedDisplay>}
};
#define NUM_ELEMENTS sizeof(ElementTable) / sizeof(ElementTable[0])

BasicAttribute::BasicAttribute (Element *parent)
        : Attribute(parent)
{
    clr = 0;
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

void BasicAttribute::dump()
{
    int indent_level = this->parent()->level();
    std::string indent = std::string(indent_level * 4, ' ');
    std::cout << indent << "Basic Attribute" << std::endl;
    std::cout << indent << "    clr: " << this->clr << std::endl;
    std::cout << indent << "    width: " << this->clr << std::endl;
    std::cout << indent << "    fill: " << this->fill << std::endl;
    std::cout << indent << "    style: " << this->style << std::endl;
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

void Limits::dump()
{

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

void Control::dump()
{

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

void Monitor::dump()
{
    int indent_level = parent()->level();
    std::string indent(indent_level * 4, ' ');
    std::cout << indent << "Monitor" << std::endl;
    std::cout << indent << "    clr: " << this->clr << std::endl;
    std::cout << indent << "    bclr: " << this->bclr << std::endl;
    std::cout << indent << "    readback: " << this->rdbk << std::endl;
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

void RelatedDisplayEntry::dump()
{

}

void RelatedDisplayEntry::toQML(std::ostream &ostream)
{
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
    : Element(parent)
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

ColorMap::ColorMap()
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

void ColorMap::dump()
{
    std::cout << "Color Map " << colormap.size() << std::endl;
    for (std::vector<std::string>::iterator it=colormap.begin(); it != colormap.end(); ++it)
        std::cout << *it << ",";
    std::cout << std::endl;
}

void Display::toQML(std::ostream &ostream)
{
    ostream << "import QtQuick 2.0\n";
    ostream << "import QtQuick.Controls 1.0\n";
    ostream << "import QtQuick.Window 2.0\n";
    ostream << "import PvComponents 1.0\n";
    ostream << "Window {\n";
    Element::toQML(ostream);
    ostream << "    color: \"" << color(this->bclr) << '"' << std::endl;

    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it)
        (*it)->toQML(ostream);

    ostream << "}\n";
}

void Display::dump()
{
    std::cout << "File" << std::endl;
    std::cout << "    name: " << this->file << std::endl;
    std::cout << "    version: " << this->version << std::endl;

    std::cout << "Display" << std::endl;
    std::cout << "    clr: " << this->clr << std::endl;
    std::cout << "    bclr: " << this->bclr << std::endl;

    this->colormap.dump();

    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it)
        (*it)->dump();
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
            this->file = token;
            FileInfo fi(this->display()->fileName());
            this->parseCompositeFile(fi.getFile(token).absolutePath().c_str());
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

void Composite::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "Item {" << std::endl;
    Element::toQML(ostream);
    this->dynamic_attr.toQML(ostream);

    for (std::list<Element*>::iterator it=widgets.begin(); it != widgets.end(); ++it)
        (*it)->toQML(ostream);

    ostream << indent << "}" << std::endl;
}

void Composite::dump()
{

}


Rectangle::Rectangle (Element *parent)
    : Element(parent),
      basic_attr(this),
      dynamic_attr(this)
{

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

void Rectangle::dump()
{

}

void Rectangle::toQML(std::ostream &ostream)
{
    std::string indent(level() * 4, ' ');

    ostream << indent << "CaRect {" << std::endl;
    Element::toQML(ostream);
    this->basic_attr.toQML(ostream);
    this->dynamic_attr.toQML(ostream);
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
    ostream << indent << "    text: " << '"' << this->label << '"' << std::endl;
    ostream << indent << "}" << std::endl;
}

void Text::dump()
{ 
    int indent_level = level() - 1;
    std::string indent(indent_level * 4, ' ');
    std::cout << indent << "Text" << std::endl;
    this->basic_attr.dump();

}

Menu::Menu (Element *parent)
    : Element(parent),
      control(this)
{
    this->_type = DL_TextEntry;
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

void Menu::dump()
{

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
    this->_type = DL_TextEntry;
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

void MessageButton::dump()
{

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
        ostream << indent << "    onMessage: " << this->onMessage << std::endl;
    if (!this->offMessage.empty())
        ostream << indent << "    offMessage: " << this->offMessage << std::endl;

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

void TextEntry::dump()
{
    int indent_level = level() - 1;
    std::string indent(indent_level * 4, ' ');
    std::cout << indent << "Text Entry" << std::endl;

    this->control.dump();
    this->limits.dump();
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

void TextUpdate::dump()
{
    int indent_level = level() - 1;
    std::string indent(indent_level * 4, ' ');
    std::cout << indent << "Text Update" << std::endl;
    this->monitor.dump();
    this->limits.dump();
}

RelatedDisplay::RelatedDisplay(Element *parent)
    : Element(parent)
{
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

void RelatedDisplay::dump()
{

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
