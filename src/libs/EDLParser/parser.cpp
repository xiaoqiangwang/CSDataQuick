#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cctype>
#include <iomanip>

#include "parser.h"

struct ObjectTable_t {
    const char* name;
    ObjectType type;
} ObjectTable [] = {
/* graphics */
    {"activeArcClass", EL_Arc},
    {"activeCircleClass", EL_Circle},
    {"activePngClass", EL_Image},
    {"cfcf6c8a_dbeb_11d2_8a97_00104b8742df", EL_Image}, // GIF Image
    {"activeLineClass", EL_Lines},
    {"activeRectangleClass", EL_Rectangle},
    {"activeXTextClass", EL_Text},
    {"activeXRegTextClass", EL_Text},
/* monitor */
    {"activeBarClass", EL_Bar},
    {"ByteClass", EL_Byte},
    {"xyGraphClass", EL_CartesianPlot},
    {"indicator", EL_Indicator},
    {"activeMeterClass", EL_Meter},
    {"StripClass", EL_StripChart},
    {"activeXTextDspClass:noedit", EL_TextUpdate},
    {"TextupdateClass", EL_TextUpdate},
    {"RegTextupdateClass", EL_TextUpdate},
/* control*/
    {"activeChoiceButton", EL_ChoiceButton},
    {"activeMenuButtonClass", EL_Menu},
    {"activeMessageButtonClass", EL_MessageButton},
    {"activeButtonClass", EL_MessageButton},
    {"shellCmdClass", EL_ShellCommand},
    {"activeSliderClass", EL_Slider},
    {"activeMotifSliderClass", EL_Slider},
    {"TextentryClass", EL_TextEntry},
    {"activeXTextDspClass", EL_TextEntry},
    {"wheel switch", EL_WheelSwitch},
/* misc */
    {"activeGroupClass", EL_Composite},
    {"activePipClass", EL_Composite},
    {"activeDynSymbolClass", EL_Symbols},
    {"relatedDisplayClass", EL_RelatedDisplay}
};
#define NUM_ELEMENTS sizeof(ObjectTable) / sizeof(ObjectTable[0])

TOKEN Object :: getToken(std::istream &fstream, char *word)
{
    enum {NEUTRAL,INCOMMENT,INQUOTE,INWORD,INMACRO} state = NEUTRAL, savedState = NEUTRAL;
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
            case '{' : return(T_LEFT_BRACE);
            case '}' : return(T_RIGHT_BRACE);
            case '#' : state = INCOMMENT;
                break;
            case '"' : state = INQUOTE;
                break;
            case '$' : c=fstream.get();
                /* only do macro substitution if in execute mode or
           * parsing a composite file */
                if( c == '(' ) {
                    savedState = NEUTRAL;
                    state = INMACRO;
                } else {
                    *w++ = '$';
                    *w++ = c;
                }
                break;
            case ' ' :
            case '\t':
            case '\r': break;
            case '\n': *word = '\0'; return T_NEWLINE;

            default  : state = INWORD;
                *w++ = c;
                break;
            }
            break;
        case INCOMMENT:
            switch(c) {
            case '\n': state = NEUTRAL; *w = '\0'; return(T_COMMENT);
            default  : *w++ = c; break;
            }
            break;
        case INQUOTE:
            switch(c) {
            case '\\': c=fstream.get();
                /* escape sequence */
                if ( c == '"' ) {
                    *w++ = '"';
                } else if ( c == 'n' ) {
                    *w++ = '\n';
                } else if ( c == 'r' ) {
                    *w++ = '\r';
                } else if ( c == 't' ) {
                    *w++ = '\t';
                } else if ( c == '\\' ) {
                    *w++ = '\\';
                } else {
                    *w++ = '\\';
                    *w++ = c;
                }
                break;
            case '"' : state = NEUTRAL; *w = '\0'; return(T_WORD);
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
                if (screen()->findMacro(macro)) {
                    std::string v = screen()->macroValue(macro);
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
                    screen()->addUnmatchedMacro(macro);
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
            case '\t':
            case '\r':
            case '\n':
            case '"' : fstream.unget(); *w = '\0'; return(T_WORD);
            default  : *w++ = c;
                break;
            }
            break;
        }
    }
    return(T_EOF);
}

std::vector<std::string> Object::getLine(std::istream &fstream)
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    bool compound = false;
    std::vector<std::string> tokens;

    do {
    switch( (tokenType=getToken(fstream,token)) ) {
    case T_WORD:
        tokens.push_back(token);
        break;
    case T_LEFT_BRACE:
        compound = true;
        break;
    case T_RIGHT_BRACE:
        compound = false;
        break;
    default:
        break;
    }
    } while ( tokenType != T_EOF && (tokenType != T_NEWLINE || compound || tokens.size() == 0) );

    return tokens;
}

bool Object::getBool(std::string boolname)
{
    auto it = properties.find(boolname);
    if (it == properties.end()) {
        return false;
    }
    
    return it->second[0] == "1";
}

std::string Object::getColor(std::string colorname)
{

    auto it = properties.find(colorname);
    if (it == properties.end()) {
        return "";
    }

    std::string color;

    auto colors = it->second;
    if (colors[0] == "index")
        color = "ColorMap.color" + colors[1];
    else if (colors[0] == "rgb") {
        std::stringstream sstream;
        sstream << "'#" << std::setfill('0') << std::setw(4) << std::hex
            << std::stoi(colors[1])
            << std::stoi(colors[2])
            << std::stoi(colors[3])
            << "'";
        color = sstream.str();
    }

    return color;
}

std::string Object::getDynamicAttribute(std::string indent)
{
    std::string alarmPv = getText("alarmPv");
    std::string visPv = getText("visPv");
    std::string min = getText("visMin");
    std::string max = getText("visMax");
    std::string visInvert = getBool("visInvert") ? "!" : "";

    std::string da;
    if (!alarmPv.empty()) {
        da += indent + "    dynamicAttribute.channel: '" + alarmPv + "'\n";
    }

    if (!visPv.empty() && !min.empty() && !max.empty()) {
        da += indent + "    dynamicAttribute.channelB: '" + visPv + "'\n";
        da += indent + "    dynamicAttribute.visibilityMode: VisibilityMode.Calc\n";
        da += indent + "    dynamicAttribute.visibilityCalc: '" + visInvert + "(B>=" + min + "&&B<" + max + ")'\n";
    }

    return da;
}

std::string Object::getFont(std::string fontname)
{
    auto it = properties.find(fontname);
    if (it == properties.end()) {
        return "";
    }
    auto fonts = it->second[0];

    // font name is of "helvetica-medium-r-14.0" format 
    // i.e. family, weight, style, size separated by "-"
    std::stringstream sstream(fonts);
    std::string item;
    std::vector<std::string> items;
    while (std::getline(sstream, item, '-')) {
        items.push_back(item); 
    }

    // captitalize weight string
    items[1][0] = std::toupper(items[1][0]);

    std::string font = "font {";
    font += "family: \"" + items[0] + "\"; ";
    font += "weight: Font." + items[1] + "; ";
    if (items[2] == "i")
    font += "italic: true; ";
    font += "pixelSize: " + items[3];
    font += " }";
    return font;
}

std::string Object::getText(std::string textname)
{
    auto it =  properties.find(textname);
    if (it == properties.end()) {
        return "";
    }
   
    auto texts = it->second;
    
    std::ostringstream osstream;
    for (auto it = texts.begin(); it != texts.end(); it++) {
        osstream << *it;
        if (texts.size() > 1 && it != --texts.end())
            osstream << "\\n";
    }

    return osstream.str();
}


void Object::parse(std::istream &fstream)
{
    std::vector<std::string> tokens;

    tokens = getLine(fstream);
    if (tokens.at(0) != "beginObjectProperties")
        return;

    tokens = getLine(fstream);
    while ( tokens.size() > 0 && tokens[0] != "endObjectProperties") {
        /* parse common geometry properties */
        if (tokens[0] == "x")
            _rect.x = std::stoi(tokens.at(1));
        else if (tokens[0] == "y")
            _rect.y = std::stoi(tokens.at(1));
        else if (tokens[0] == "w")
            _rect.width = std::stoi(tokens.at(1));
        else if (tokens[0] == "h")
            _rect.height = std::stoi(tokens.at(1));
        else if (tokens[0] == "major")
            _version |= std::stoi(tokens[1]) << 16;
        else if (tokens[0] == "minor")
            _version |= std::stoi(tokens[1]) << 8;
        else if (tokens[0] == "release")
            _version |= std::stoi(tokens[1]);
        else if (tokens[0] == "beginGroup") {
            this->parseGroup(fstream);
        }
        else if (tokens.size() == 1)
            this->properties[tokens[0]].push_back("1");
        else {
            for (int i=1; i<tokens.size(); i++)
                this->properties[tokens[0]].push_back(tokens[i]);
        }
        tokens = getLine(fstream);
    }
}

void Object::parseAndSkip(std::istream &fstream)
{
    std::vector<std::string> tokens;
    tokens = getLine(fstream);
    while ( tokens.size() > 0 && tokens[0] != "endObjectProperties") {
        tokens = getLine(fstream);
    }
}

void Object::parseGroup(std::istream &fstream)
{
    std::vector<std::string> tokens;
    tokens = getLine(fstream);
    while ( tokens.size() > 0 && tokens[0] != "endGroup") {
        bool found = false;
        if (tokens[0] == "object") {
            for (size_t i = 0; i < NUM_ELEMENTS; i++) {
                if (tokens[1] == ObjectTable[i].name) {
                    Object *object = new Object(ObjectTable[i].type, this);
                    object->parse(fstream);
                    objects.push_back(object);
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            std::cout << tokens[1] << " Not Implemented" << std::endl;
            parseAndSkip(fstream);
        }
        
        tokens = getLine(fstream);
    }
}

void Object::textToQML(std::ostream& ostream)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::string attr;

    int indent_level = level();
    std::string indent(indent_level * 4, ' ');
    
    ostream << indent << "CSText {" << std::endl;
    rectToQML(ostream);

    attr = getText("value");
    ostream << indent << "    text: '" << attr << "'" << std::endl;

    it = properties.find("fontAlign");
    if (it != properties.end()) {
        std::string align = properties["fontAlign"][0];
        std::string qml = "";
        if (align == "left")
            qml = "Text.AlignLeft";
        else if (align == "center")
            qml = "Text.AlignHCenter";
        else if (align == "right")
            qml = "Text.AlignRight";
        if (!qml.empty())
            ostream << indent << "    align: " <<  qml << std::endl;
    }

    attr = getFont("font");
    if (!attr.empty())
        ostream << indent << "    " << attr << std::endl;

    attr = getColor("fgColor");
    if (!attr.empty())
        ostream << indent << "    foreground: " <<  attr << std::endl;

    if (!getBool("useDisplayBg")) {
        attr = getColor("bgColor");
        if (!attr.empty())
            ostream << indent << "    background: " <<  attr << std::endl;
    }

    attr = getDynamicAttribute(indent);
    if (!attr.empty())
        ostream << attr << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::genericToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');
    
    ostream << indent << "Item {" << std::endl;
    rectToQML(ostream);
    
    for (std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
        (*it)->toQML(ostream);
    }
 
    ostream << indent << "}" << std::endl;
}

void Object::rectToQML(std::ostream& ostream)
{
    int indent_level = level() + 1;
    std::string indent(indent_level * 4, ' ');

    int offsetX = 0, offsetY = 0;

    if (parent() && parent()->type() != EL_Screen) {
        Rect rc = parent()->rect();
        offsetX = rc.x;
        offsetY = rc.y;
    }

    ostream << indent << "x: " << this->_rect.x - offsetX << std::endl;
    ostream << indent << "y: " << this->_rect.y - offsetY << std::endl;
    ostream << indent << "width: " << this->_rect.width << std::endl;
    ostream << indent << "height: " << this->_rect.height << std::endl;
}

void Object::toQML(std::ostream& ostream)
{
    switch(type()) {
        case EL_Text:
        textToQML(ostream);
        break;
        default:
        genericToQML(ostream);
        break;
    }
}

Screen :: Screen ()
    : Object(EL_Screen, 0)
{
}

void Screen :: parse(std::istream &fstream) 
{
    char token[MAX_TOKEN_LENGTH];
    TOKEN tokenType;
    int nestingLevel = 0;
    std::vector<std::string> tokens;

    /* version */
    tokens = getLine(fstream);
    if (tokens.size() != 3) {
        std::cerr << "Wrong EDL format" << std::endl;
    }
    int version = std::stoi(tokens[0]) << 16 | std::stoi(tokens[1]) << 8 | std::stoi(tokens[2]);
    if (version < 0x040000) {
        std::cerr << "EDL version is not supported." << std::endl;
        return;
    }

    /* screen */
    this->parseScreen(fstream);

    /* objects */
    tokens = getLine(fstream);
    while ( tokens.size() > 0) {
        bool found = false;
        if (tokens[0] == "object") {
            for (size_t i = 0; i < NUM_ELEMENTS; i++) {
                if (tokens[1] == ObjectTable[i].name) {
                    Object *object = new Object(ObjectTable[i].type, this);
                    object->parse(fstream);
                    objects.push_back(object);
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            std::cerr << tokens[1] << " Not Implemented" << std::endl;
            parseAndSkip(fstream);
        }
        
        tokens = getLine(fstream);
    }
}

void Screen::parseScreen(std::istream &fstream)
{
    std::vector<std::string> tokens;

    tokens = getLine(fstream);
    if (tokens.at(0) != "beginScreenProperties")
        return;

    tokens = getLine(fstream);
    while ( tokens.size() > 0 && tokens[0] != "endScreenProperties") {
        /* parse common geometry properties */
        if (tokens[0] == "x")
            _rect.x = std::stoi(tokens.at(1));
        else if (tokens[0] == "y")
            _rect.y = std::stoi(tokens.at(1));
        else if (tokens[0] == "w")
            _rect.width = std::stoi(tokens.at(1));
        else if (tokens[0] == "h")
            _rect.height = std::stoi(tokens.at(1));
        else if (tokens[0] == "major")
            _version |= std::stoi(tokens[1]) << 16;
        else if (tokens[0] == "minor")
            _version |= std::stoi(tokens[1]) << 8;
        else if (tokens[0] == "release")
            _version |= std::stoi(tokens[1]);
        else if (tokens.size() == 1)
            this->properties[tokens[0]].push_back("1");
        else {
            for (int i=1; i<tokens.size(); i++)
                this->properties[tokens[0]].push_back(tokens[i]);
        }
        tokens = getLine(fstream);
    }
}

void Screen::toQML(std::ostream& ostream)
{
    ostream << "import QtQuick 2.0\n";
    ostream << "import CSDataQuick.Components 1.0\n";
    ostream << "BaseWindow {\n";
    Object::rectToQML(ostream);
    
    std::string attr;

    attr = getText("title");
    if (!attr.empty())
        ostream << "    title: '" <<  attr << "'" << std::endl;

    attr = getColor("bgColor");
    if (!attr.empty())
        ostream << "    color: " <<  attr << std::endl;

    for (std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
        (*it)->toQML(ostream);
    }

    ostream << "}" << std::endl;
}

void Screen::toPartialQML(std::ostream& ostream)
{

}

int main(int argc, char **argv)
{
    std::string filename = std::string(argv[1]);
    Screen screen;

    std::vector<std::string> tokens;
    std::ifstream ifstream(filename.c_str());

    screen.parse(ifstream);

    std::ostringstream osstream;
    screen.toQML(osstream);
    std::cout << osstream.str() << std::endl;
}
