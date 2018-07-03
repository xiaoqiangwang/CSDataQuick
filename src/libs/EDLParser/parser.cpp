#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <climits>
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
    {"activeCoefTableClass", EL_CoefTable},
    {"activeVsBarClass", EL_Bar},
    {"ByteClass", EL_Byte},
    {"xyGraphClass", EL_CartesianPlot},
    {"activeIndicatorClass", EL_Indicator},
    {"activeMeterClass", EL_Meter},
    {"multiLineTextUpdateClass", EL_MultilineText},
    {"StripClass", EL_StripChart},
    {"activeXTextDspClass:noedit", EL_TextUpdate},
    {"TextupdateClass", EL_TextUpdate},
    {"RegTextupdateClass", EL_TextUpdate},
/* control*/
    {"activeChoiceButtonClass", EL_ChoiceButton},
    {"activeMenuButtonClass", EL_Menu},
    {"activeMessageButtonClass", EL_MessageButton},
    {"multiLineTextEntryClass", EL_MultilineText},
    {"activeButtonClass", EL_MessageButton},
    {"shellCmdClass", EL_ShellCommand},
    {"activeSliderClass", EL_Slider},
    {"activeMotifSliderClass", EL_Slider},
    {"TextentryClass", EL_TextEntry},
    {"activeXTextDspClass", EL_TextEntry},
    {"activeUpdownButtonClass", EL_UpDownButton},
    {"wheel switch", EL_WheelSwitch},
/* misc */
    {"activeGroupClass", EL_Composite},
    {"activePipClass", EL_Composite},
    {"activeDynSymbolClass", EL_Symbols},
    {"relatedDisplayClass", EL_RelatedDisplay},
    {"activeExitButtonClass", EL_ExitButton}
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

int Object::getInteger(std::string intname, int defaultvalue)
{
    auto it = properties.find(intname);
    if (it == properties.end()) {
        return defaultvalue;
    }
    
    return std::stoi(it->second[0]);
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
    std::string alarmPv = getPv("alarmPv");
    std::string visPv = getPv("visPv");
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

std::vector<std::string> Object::getList(std::string listname)
{
    std::vector<std::string> list;

    auto it =  properties.find(listname);
    if (it != properties.end()) {
        for(int i=1; i<it->second.size(); i+=2)
            list.push_back(it->second[i]);
    }

    return list;
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

std::string Object::getPv(std::string pvname)
{
    auto it = properties.find(pvname);
    if (it == properties.end()) {
        return "";
    }
    std::string pv = it->second[0];
    std::string::size_type pos = 0;
    std::string value;

    // local pv is of form "LOC\<NAME>=<type>:<value>[,enum0,enum1...]"
    //
    if (pv.substr(0, 4) != "LOC\\")
        return pv;

    std::stringstream sstream(pv.substr(4));
    std::string name;

    if (!std::getline(sstream, name, '='))
        return "loc://" + name;
    
    std::stringstream json; 
    // find the type
    char type = sstream.get();
    if (sstream.peek() != ':') {
        type = 's';
        sstream.unget();
    } else {
        sstream.get();
    }
    // parse the value
    switch(type) {
        case 'e':
        {
            json << "{\"type\":\"enum\";";
            bool first = true;
            std::string item;
            std::vector<std::string> enums;
            while (std::getline(sstream, item, ',')) {
                if (first) {
                    json << "\"value\":" << item << ";";
                    first = false;
                } else {
                    enums.push_back(item);
                }
            }
            for (auto it = enums.begin(); it!=enums.end(); it++) {
                if (it == enums.begin())
                    json << "\"enums\":[";
                json << "\"" << *it << "\"";
                if (it == enums.end() -1)
                    json << "]";
                else
                    json << ",";
            }
            json << "}";
        }
        break;
        case 'd':
        {
            double d;
            sstream >> d;
            json << "{" << "\"type\":\"double\";" << "\"value\":" << d << "}";
        }
        break;
        case 'i':
        {
            int i;
            sstream >> i;
            json << "{" << "\"type\":\"int\";" << "\"value\":" << i << "}";
        }
        break;
        case 's':
        default:
        {
            std::string s;
            std::getline(sstream, s);
            json << "{" << "\"type\":\"string\";" << "\"value\":\"" << s << "\"}";
        }
    }

    return "loc://" + name + "." + json.str();
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

void Object::arcToQML(std::ostream& ostream)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::string attr;

    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSArc {" << std::endl;
    rectToQML(ostream);

    if (getBool("fill"))
        ostream << indent << "    foreground: " << getColor("fillColor") << std::endl;
    else {
        ostream << indent << "    foreground: " << getColor("lineColor") << std::endl;
        ostream << indent << "    fillStyle: FillStyle.Outline" << std::endl;
    }

    std::string lineWidth = getText("lineWidth");
    if (!lineWidth.empty())
        ostream << indent << "    lineWidth: " << lineWidth << std::endl;

    std::string lineStyle = getText("lineStyle");
    if (lineStyle == "dash")
        ostream << indent << "    edgeStyle: EdgeStyle.Dash" << std::endl;

    std::string begin = getText("startAngle");
    if (!begin.empty())
        ostream << indent << "    begin: " << begin << std::endl;

    std::string span = getText("totalAngle");
    if (!span.empty())
        ostream << indent << "    span: " << span << std::endl;

    attr = getDynamicAttribute(indent);
    if (!attr.empty())
        ostream << attr << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::circleToQML(std::ostream& ostream)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::string attr;

    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSOval {" << std::endl;
    rectToQML(ostream);

    if (getBool("fill"))
        ostream << indent << "    foreground: " << getColor("fillColor") << std::endl;
    else {
        ostream << indent << "    foreground: " << getColor("lineColor") << std::endl;
        ostream << indent << "    fillStyle: FillStyle.Outline" << std::endl;
    }

    std::string lineWidth = getText("lineWidth");
    if (!lineWidth.empty())
        ostream << indent << "    lineWidth: " << lineWidth << std::endl;

    std::string lineStyle = getText("lineStyle");
    if (lineStyle == "dash")
        ostream << indent << "    edgeStyle: EdgeStyle.Dash" << std::endl;


    attr = getDynamicAttribute(indent);
    if (!attr.empty())
        ostream << attr << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::imageToQML(std::ostream& ostream)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::string attr;

    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSImage {" << std::endl;
    rectToQML(ostream);

    std::string filename = getText("file");
    if (!filename.empty())
    ostream << indent << "    source: '" << filename << "'" << std::endl;
    
    ostream << indent << "}" << std::endl;
}

void Object::linesToQML(std::ostream& ostream)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::string attr;

    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    if (getBool("closePolygon") || getBool("fill"))
    ostream << indent << "CSPolygon {" << std::endl;
    else
    ostream << indent << "CSPolyline {" << std::endl;

    rectToQML(ostream);

    if (getBool("closePolygon") || getBool("fill"))
    ostream << indent << "    foreground: " << getColor("fillColor") << std::endl;
    else
    ostream << indent << "    foreground: " << getColor("lineColor") << std::endl;

    std::string lineWidth = getText("lineWidth");
    if (!lineWidth.empty())
        ostream << indent << "    lineWidth: " << lineWidth << std::endl;

    std::string lineStyle = getText("lineStyle");
    if (lineStyle == "dash")
        ostream << indent << "    edgeStyle: EdgeStyle.Dash" << std::endl;

    auto xp = getList("xPoints");
    auto yp = getList("yPoints");

    ostream << indent << "    points: [";
    for (int i=0; i<xp.size(); i++) {
        ostream << "Qt.point(" << std::stoi(xp[i]) - this->_rect.x << ","
            << std::stoi(yp[i]) - this->_rect.y << ")";
        if (i != xp.size() -1)
            ostream << ",";
    }
    ostream << "]" << std::endl;

    std::string arrows = getText("arrows");
    if (arrows == "from")
        ostream << indent << "    arrowPosition: ArrowPosition.Start" << std::endl;
    else if (arrows == "to")
        ostream << indent << "    arrowPosition: ArrowPosition.End" << std::endl;
    else if (arrows == "both")
        ostream << indent << "    arrowPosition: ArrowPosition.Both" << std::endl;

    attr = getDynamicAttribute(indent);
    if (!attr.empty())
        ostream << attr << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::compositeToQML(std::ostream& ostream)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::string attr;

    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSComposite {" << std::endl;
    rectToQML(ostream);

    std::string displaySource = getText("displaySource");
    if (displaySource == "stringPv" || displaySource == "menu") {
        std::cerr << "activePipClass: displaySource '" << displaySource << "' not supported" << std::endl;
    } else if (displaySource == "file") {
        std::string filename = getText("file");
        if (!filename.empty()) {
            if (filename.substr(filename.size() - 4) != ".edl")
                filename += ".edl";
            ostream << indent << "    source: '" << filename << "'" << std::endl;
            std::string macro = screen()->macroString();
            if (!macro.empty())
            ostream << indent << "    macro: '" << macro << "'" << std::endl;
        }
    } else {
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            (*it)->toQML(ostream);
        }
        attr = getDynamicAttribute(indent);
        if (!attr.empty())
            ostream << attr << std::endl;
    }

    ostream << indent << "}" << std::endl;
}

void Object::rectangleToQML(std::ostream& ostream)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::string attr;

    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSRect {" << std::endl;
    rectToQML(ostream);

    if (getBool("fill"))
        ostream << indent << "    foreground: " << getColor("fillColor") << std::endl;
    else {
        ostream << indent << "    foreground: " << getColor("lineColor") << std::endl;
        ostream << indent << "    fillStyle: FillStyle.Outline" << std::endl;
    }

    std::string lineWidth = getText("lineWidth");
    if (!lineWidth.empty())
        ostream << indent << "    lineWidth: " << lineWidth << std::endl;

    std::string lineStyle = getText("lineStyle");
    if (lineStyle == "dash")
        ostream << indent << "    edgeStyle: EdgeStyle.Dash" << std::endl;

    attr = getDynamicAttribute(indent);
    if (!attr.empty())
        ostream << attr << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::relatedDisplayToQML(std::ostream& ostream)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    std::string attr;

    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSRelatedDisplay {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    label: '" << getText("buttonLabel") << "'" << std::endl;

    auto entries = getList("displayFileName");
    auto labels = getList("menuLabel");
    auto symbols = getList("symbols");
    auto closeActions = getList("closeAction");

    if (entries.size() > 0) {
        ostream << indent << "    model: ListModel {" << std::endl;
        for(int i=0; i<entries.size(); i++) {
            ostream << indent << "        ListElement {" << std::endl;
            std::string filename = entries[i];
            if (filename.substr(filename.size() - 4) != ".edl")
                filename += ".edl";
            ostream << indent << "              file: '" <<  filename << "'" << std::endl;
            ostream << indent << "              label: '" << (i < labels.size() ? labels[i] : "") << "'" << std::endl;
            std::string macro = screen()->macroString();
            if (i < symbols.size() && !symbols[i].empty())
                macro += (macro.empty() ? "" : ",") + symbols[i]; 
            if (!macro.empty())
            ostream << indent << "              macro: '" << macro << std::endl;
            ostream << "'" << std::endl;
            if (i < closeActions.size() && closeActions[i] == "1")
            ostream << indent << "              replace: true" << std::endl;
            ostream << indent << "        }" << std::endl;
        }
        ostream << indent << "    }" << std::endl;
    }

    ostream << indent << "}" << std::endl;
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
    std::cerr << "Type " << this->type() << " using generic output" << std::endl;
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "Item {" << std::endl;
    rectToQML(ostream);

    for (std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it) {
        (*it)->toQML(ostream);
    }
 
    ostream << indent << "}" << std::endl;
}

void Object::barToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSBar {" << std::endl;
    rectToQML(ostream);

    std::string fgColor = getColor("indicatorColor");
    if (fgColor.empty())
        fgColor = getColor("indicatorColour");
    ostream << indent << "    foreground: " << fgColor << std::endl;

    std::string bgColor = getColor("bgColor");
    if (bgColor.empty())
        bgColor = getColor("bgColour");
     ostream << indent << "    background: " << bgColor << std::endl;

    if (getBool("fgAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

    std::string orien = getText("orientation");
    if (orien == "vertical")
        ostream << indent << "    direction: Direction.Up" << std::endl;

    if (!getText("origin").empty())
    ostream << indent << "    fillMode: FillMode.FromCenter" << std::endl;

    // activeVsBarClass (Variable Scale Bar)
    std::string maxPv = getPv("maxPv");
    if (!maxPv.empty()) {
        ostream << indent << "    property var _maxPv: CSData {" << std::endl;
        ostream << indent << "        source: '" << maxPv << "'" << std::endl;
        ostream << indent << "    }" << std::endl;
        ostream << indent << "    limits.loprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.hoprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.precSrc: LimitsSource.Default" << std::endl;

        ostream << indent << "    limits.hoprDefault: _maxPv.value === undefined ? 1 : _maxPv.value" << std::endl;
        ostream << indent << "    limits.precDefault: " << getInteger("precision") << std::endl;
    } else if (!getBool("limitsFromDb")) {
        ostream << indent << "    limits.loprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.hoprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.precSrc: LimitsSource.Default" << std::endl;

        ostream << indent << "    limits.loprDefault: " << getInteger("min") << std::endl;
        ostream << indent << "    limits.hoprDefault: " << getInteger("max", 1) << std::endl;
        ostream << indent << "    limits.precDefault: " << getInteger("precision") << std::endl;
    }

    if (getText("labelType") == "pvName")
    ostream << indent << "    labelStyle: LabelStyle.Channel" << std::endl;
    else if (getBool("showScale"))
    ostream << indent << "    labelStyle: LabelStyle.Outline" << std::endl;

    ostream << indent << "    source: '" << getPv("indicatorPv") << "'" << std::endl;
    ostream << indent << "}" << std::endl;
}

void Object::byteToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSByte {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    foreground: " << getColor("onColor") << std::endl;
    ostream << indent << "    background: " << getColor("offColor") << std::endl;

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;

    std::string shiftS = getText("shift");
    int shift = 0;
    if (!shiftS.empty())
        shift = std::stoi(shiftS);

    std::string numBitsS = getText("numBits");
    int numBits = 16;
    if (!numBitsS.empty())
        numBits = std::stoi(numBitsS);

    std::string endian = getText("endian");
    if (endian == "little") {
        ostream << indent << "    start: " << shift + numBits - 1 << std::endl;
        ostream << indent << "    end: " << shift << std::endl;
    } else {
        ostream << indent << "    start: " << shift + numBits - 1 << std::endl;
        ostream << indent << "    end: " << shift << std::endl;
    }
    ostream << indent << "}" << std::endl;
}

void Object::coefTableToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSTextEntryArray {" << std::endl;
    rectToQML(ostream);
    
    ostream << indent << "    source: '" << getText("pv") << "'" << std::endl;
    ostream << indent << "    index: " << getInteger("firstElement") << std::endl;
    ostream << indent << "    count: " << getInteger("numElements", 1) << std::endl;
    ostream << indent << "}" << std::endl;
}

void Object::indicatorToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSIndicator {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    foreground: " << getColor("indicatorColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;

    if (getBool("fgAlarm") || getBool("indicatorAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

    std::string orien = getText("orientation");
    if (orien == "vertical")
    ostream << indent << "    direction: Direction.Up" << std::endl;

    if (!getBool("limitsFromDb")) {
    ostream << indent << "    limits.loprSrc: LimitsSource.Default" << std::endl;
    ostream << indent << "    limits.hoprSrc: LimitsSource.Default" << std::endl;
    ostream << indent << "    limits.precSrc: LimitsSource.Default" << std::endl;

    ostream << indent << "    limits.loprDefault: " << getInteger("min") << std::endl;
    ostream << indent << "    limits.hoprDefault: " << getInteger("max", 1) << std::endl;
    ostream << indent << "    limits.precDefault: " << getInteger("precision") << std::endl;
    }

    if (getText("labelType") == "pvName")
    ostream << indent << "    labelStyle: LabelStyle.Channel" << std::endl;
    else if (getBool("showScale"))
    ostream << indent << "    labelStyle: LabelStyle.Outline" << std::endl;

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::meterToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSMeter {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;

    if (!getBool("scaleLimitsFromDb")) {
        ostream << indent << "    limits.loprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.hoprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.precSrc: LimitsSource.Default" << std::endl;

        ostream << indent << "    limits.loprDefault: " << getInteger("scaleMin") << std::endl;
        ostream << indent << "    limits.hoprDefault: " << getInteger("scaleMax", 1) << std::endl;
        ostream << indent << "    limits.precDefault: " << getInteger("scalePrecision") << std::endl;
    }

    ostream << indent << "    source: '" << getPv("readPv") << "'" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::textUpdateToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSTextUpdate {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;
    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;

    if (getBool("fgAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

    std::string format = getText("displayMode");
    if (format == "decimal")
    ostream << indent << "    format: TextFormat.Decimal" << std::endl;
    else if (format == "hex")
    ostream << indent << "    format: TextFormat.Hexadecimal" << std::endl;
    else if (format == "engineer")
    ostream << indent << "    format: TextFormat.EngNotation" << std::endl;
    else if (format == "exp")
    ostream << indent << "    format: TextFormat.Exponential" << std::endl;
    else
    ostream << indent << "    format: TextFormat.String" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::choiceButtonToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSChoiceButton {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;
    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;
    if (getBool("fgAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::menuToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSMenu {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;
    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;
    if (getBool("fgAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::messageButtonToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSMessageButton {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;
    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("onColor") << std::endl;
    ostream << indent << "    text: '" << getText("onLabel") << "'" << std::endl;

    std::string pressValue = getText("pressValue");
    if (!pressValue.empty())
    ostream << indent << "    onMessage: '" << pressValue << "'" << std::endl;
    std::string releaseValue = getText("releaseValue");
    if (!releaseValue.empty())
    ostream << indent << "    offMessage: '" << releaseValue << "'" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::multilineTextToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSTextArea {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    foreground: " << getColor("fgColour") << std::endl;
    ostream << indent << "    background: " << getColor("bgColour") << std::endl;
    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;

    std::string font = getFont("font");
    if (!font.empty())
        ostream << indent << "    " << font << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::shellCommandToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSShellCommand {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;
    ostream << indent << "    label: '" << getText("buttonLabel") << "'" << std::endl;

    int numCmds = std::stoi(getText("numCmds"));
    auto commands = getList("command");
    auto labels = getList("commandLabel");

    ostream << indent << "    model: ListModel {" << std::endl;
    for(int i=0; i<numCmds; i++) {
    ostream << indent << "        ListElement {" << std::endl;
    ostream << indent << "            command: '" << commands[i] << "'" << std::endl;
    ostream << indent << "            label: '" << (i < labels.size() ? labels[i] : "") << "'" << std::endl;
    ostream << indent << "        }" << std::endl;
    }
    ostream << indent << "    }" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::sliderToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSSlider {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;

    if (!getBool("scaleLimitsFromDb")) {
        ostream << indent << "    limits.loprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.hoprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.precSrc: LimitsSource.Default" << std::endl;

        ostream << indent << "    limits.loprDefault: " << getText("scaleMin") << std::endl;
        ostream << indent << "    limits.hoprDefault: " << getText("scaleMax") << std::endl;
        ostream << indent << "    limits.precDefault: " << getText("precision") << std::endl;
    }

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::textEntryToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    if (getBool("editable"))
    ostream << indent << "CSTextEntry {" << std::endl;
    else
    ostream << indent << "CSTextUpdate {" << std::endl;

    rectToQML(ostream);

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;
    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;
    if (getBool("fgAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;
    ostream << indent << "    format: TextFormat.String" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::updownButtonToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSSpinBox {" << std::endl;

    rectToQML(ostream);

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;
    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;

    if (!getBool("scaleLimitsFromDb")) {
        ostream << indent << "    limits.loprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.hoprSrc: LimitsSource.Default" << std::endl;

        ostream << indent << "    limits.loprDefault: " << getText("scaleMin") << std::endl;
        ostream << indent << "    limits.hoprDefault: " << getText("scaleMax") << std::endl;
    }

    ostream << indent << "}" << std::endl;
}

void Object::exitButtonToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "BaseItem {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    StyledButton {" << std::endl;
    ostream << indent << "        anchors.fill: parent" << std::endl;
    ostream << indent << "        foregroundColor: " << getColor("fgColor") << std::endl;
    ostream << indent << "        backgroundColor: " << getColor("bgColor") << std::endl;
    ostream << indent << "        text: '" << getText("label") << "'" << std::endl;
    ostream << indent << "        onClicked: parent.baseWindow.close()" << std::endl;
    ostream << indent << "     }" << std::endl;
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
        case EL_Arc:
        arcToQML(ostream);
        break;
        case EL_Circle:
        circleToQML(ostream);
        break;
        case EL_Image:
        imageToQML(ostream);
        break;
        case EL_Lines:
        linesToQML(ostream);
        break;
        case EL_Text:
        textToQML(ostream);
        break;
        case EL_Composite:
        compositeToQML(ostream);
        break;
        case EL_Rectangle:
        rectangleToQML(ostream);
        break;
        case EL_Bar:
        barToQML(ostream);
        break;
        case EL_Byte:
        byteToQML(ostream);
        break;
        case EL_CoefTable:
        coefTableToQML(ostream);
        break;
        case EL_Indicator:
        indicatorToQML(ostream);
        break;
        case EL_Meter:
        meterToQML(ostream);
        break;
        case EL_TextUpdate:
        textUpdateToQML(ostream);
        break;
        case EL_ChoiceButton:
        choiceButtonToQML(ostream);
        break;
        case EL_Menu:
        menuToQML(ostream);
        break;
        case EL_MessageButton:
        messageButtonToQML(ostream);
        break;
        case EL_MultilineText:
        multilineTextToQML(ostream);
        break;
        case EL_ShellCommand:
        shellCommandToQML(ostream);
        break;
        case EL_TextEntry:
        textEntryToQML(ostream);
        break;
        case EL_RelatedDisplay:
        relatedDisplayToQML(ostream);
        break;
        case EL_UpDownButton:
        updownButtonToQML(ostream);
        break;
        case EL_ExitButton:
        exitButtonToQML(ostream);
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
        std::cerr << file << ": EDL version " << tokens[0] << "." << tokens[1] << "." << tokens[2] << " is not supported." << std::endl;
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
    ostream << "import CSDataQuick.Data 1.0\n";
    ostream << "import CSDataQuick.Components 1.0\n";
    ostream << "import CSDataQuick.Components.Private 1.0\n";
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
    /* find out the bounding rect */
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        Object *object = (*it);
        Rect rc = object->rect();
        minX = std::min(rc.x, minX);
        minY = std::min(rc.y, minY);
        maxX = std::max(rc.x + rc.width, maxX);
        maxY = std::max(rc.y + rc.height, maxY);
    }

    /* enlarge composite if necessary */
    _rect.width = std::max(maxX - minX, _rect.width);
    _rect.height = std::max(maxY - minY, _rect.height);

    /* shift all components accordingly */
    int offsetX = - minX, offsetY =  - minY;
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        Object *object = (*it);
        Rect rc = object->rect();
        rc.x = rc.x + offsetX;
        rc.y = rc.y + offsetY;
        object->setRect(rc);

        object->toQML(ostream);
    }
}
