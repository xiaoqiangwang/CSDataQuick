/*****************************************************************************\
 * Copyright 2018 Paul Scherrer Institut, Villigen, Switzerland
\*****************************************************************************/

#include <algorithm>
#include <iterator>
#include <climits>
#include <cctype>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <fstream>

#include <string.h>

#include "edl_parser.h"

namespace EDL {

/* EDM class name to object type conversion table */
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
    int c;
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
            case '\n': state = NEUTRAL; *w = '\0'; fstream.unget(); return(T_COMMENT);
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
    bool compound = false, done = false;
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
    case T_NEWLINE:
        if (compound) {
            if (tokens.size() > 1)
                tokens.push_back("\n");
        } else {
            done = (tokens.size() > 0);
        }
        break;
    case T_COMMENT:
        if (tokens.size() > 0)
            tokens.push_back(std::string("#")+token);
        break;
    case T_EOF:
        done = true;
        break;
    default:
        break;
    }
    } while ( !done );

    if (tokens.size() > 0 && tokens.back() == "\n")
        tokens.pop_back();
    return tokens;
}

bool Object::getBool(std::string boolname)
{
    auto it = properties.find(boolname);
    if (it == properties.end()) {
        return false;
    }

    auto values = it->second;
    if (values.size() < 1)
        return true;
    else
        return values[0] == "1";
}

int Object::getInteger(std::string intname, int defaultvalue)
{
    auto it = properties.find(intname);
    if (it == properties.end()) {
        return defaultvalue;
    }

    auto values = it->second;
    if (values.size() < 1)
        return defaultvalue;
    else
        return std::stoi(values[0]);
}

std::string Object::getColor(std::vector<std::string> colors)
{
    std::string color;
    if (colors[0] == "index") {
        color = screen()->color(std::stoi(colors[1]));
        if (color.empty())
            color = "ColorMap.color" + colors[1];
        else
            color = "'" + color + "'";
    }
    else if (colors[0] == "rgb") {
        std::stringstream sstream;
        sstream << "'#" << std::setfill('0') << std::hex
            << std::setw(4) << std::stoi(colors[1])
            << std::setw(4) << std::stoi(colors[2])
            << std::setw(4) << std::stoi(colors[3])
            << "'";
        color = sstream.str();
    }

    return color;
}

std::string Object::getColor(std::string colorname)
{

    auto it = properties.find(colorname);
    if (it == properties.end()) {
        return "";
    }
    return getColor(it->second);
}

std::vector<std::string> Object::getColorList(std::string colorname)
{
    std::vector<std::string> colorlist;

    auto it = properties.find(colorname);
    if (it == properties.end()) {
        return colorlist;
    }

    auto colors = it->second;
    auto cit = colors.begin();
    while (cit != colors.end()) {
        int index = std::stoi(*cit);
        while (index >= colorlist.size())
            colorlist.push_back("'black'");
        cit++;
        if ((*cit) == "index") {
            std::vector<std::string> color;
            std::copy_n(cit, 2, std::back_inserter(color));
            colorlist[index] = getColor(color);
            cit += 2;
        } else if ((*cit) == "rgb") {
            std::vector<std::string> color;
            std::copy_n(cit, 4, std::back_inserter(color));
            colorlist[index] = getColor(color);
            cit += 4;
        }
    }
    return colorlist;
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
        auto tokens = it->second;
        int i = 0;
        while (i < tokens.size()) {
            int index = std::stoi(tokens[i]);
            std::string value;
            while (++i < tokens.size() && tokens[i] != "\n") {
                if (value.size() > 0) value += " ";
                value += tokens[i];
            }
            i++;
            /* Fill skipped values */
            while (index >= list.size())
                list.push_back("");

            list[index] = value;
        }
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
    }

    return osstream.str();
}

std::string Object::parsePv(std::string pv)
{
    std::string::size_type pos = 0;
    std::string value;

    // pv is of form "[EPICS\|LOC|\CALC\]<NAME>=<type>:<value>[,enum0,enum1...]"
    pos = pv.find('\\');
    if (pos == std::string::npos)
        return pv;

    std::string pvtype = pv.substr(0, pos);
    if (pvtype == "EPICS")
        return pv.substr(pos+1);
    else if (pvtype != "LOC") {
        std::cerr << "Unsupported PV type '" << pvtype << "': " << pv << std::endl;
        return "";
    }

    std::stringstream sstream(pv.substr(4));
    std::string name;

    std::getline(sstream, name, '=');
    
    if (sstream.eof())
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
            json << "{\"type\":\"enum\",";
            bool first = true;
            std::string item;
            std::vector<std::string> enums;
            while (std::getline(sstream, item, ',')) {
                if (first) {
                    json << "\"value\":" << item << ",";
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
            json << "{" << "\"type\":\"double\"," << "\"value\":" << d << "}";
        }
        break;
        case 'i':
        {
            int i;
            sstream >> i;
            json << "{" << "\"type\":\"int\"," << "\"value\":" << i << "}";
        }
        break;
        case 's':
        default:
        {
            std::string s;
            std::getline(sstream, s);
            json << "{" << "\"type\":\"string\"," << "\"value\":\"" << s << "\"}";
        }
    }

    return "loc://" + name + "." + json.str();
}


std::string Object::getPv(std::string pvname)
{
    auto it = properties.find(pvname);
    if (it == properties.end()) {
        return "";
    }
    return parsePv(it->second[0]);
}

std::vector<std::string> Object::getPvList(std::string listname)
{
    std::vector<std::string> pvlist;

    auto it =  properties.find(listname);
    if (it != properties.end()) {
        auto tokens = it->second;
        for(int i=0; i<it->second.size(); i+=2) {
            int index = std::stoi(tokens[i]);
            std::string pv = parsePv(tokens[i+1]);
            while (index >= pvlist.size())
                pvlist.push_back("");
            pvlist[index] = pv;
        }
    }

    return pvlist;
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
        else if (tokens[0] == "beginGroup")
            this->parseGroup(fstream);
        else {
            std::copy(tokens.begin() + 1,
                    tokens.end(),
                    std::back_inserter(this->properties[tokens[0]]));
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
            if (!found) {
                std::cout << tokens[1] << " Not Implemented" << std::endl;
                parseAndSkip(fstream);
            }
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

    if (getBool("fill")) {
        ostream << indent << "    foreground: " << getColor("fillColor") << std::endl;
        std::string fillMode = getText("fillMode");
        if (fillMode == "pie")
        ostream << indent << "    closure: Arc.Pie" << std::endl;
        else
        ostream << indent << "    closure: Arc.Chord" << std::endl;
    }
    else {
        ostream << indent << "    foreground: " << getColor("lineColor") << std::endl;
        ostream << indent << "    fillStyle: FillStyle.Outline" << std::endl;
    }

    if (getBool("lineAlarm") || getBool("fillAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

    std::string lineWidth = getText("lineWidth");
    if (!lineWidth.empty())
        ostream << indent << "    lineWidth: " << lineWidth << std::endl;

    std::string lineStyle = getText("lineStyle");
    if (lineStyle == "dash")
        ostream << indent << "    edgeStyle: EdgeStyle.Dash" << std::endl;

    ostream << indent << "    begin: " << getInteger("startAngle") << std::endl;
    ostream << indent << "    span: " << getInteger("totalAngle", 180) << std::endl;

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
    if (getBool("lineAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

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

    // fix zero width/height for straight lines
    if (this->_rect.width <= 1)
        this->_rect.width = std::max(1, getInteger("lineWidth"));
    if (this->_rect.height <= 1)
        this->_rect.height = std::max(1, getInteger("lineWidth"));
    this->_rect.x -= getInteger("lineWidth")/2;
    this->_rect.y -= getInteger("lineWidth")/2;
    this->_rect.width += getInteger("lineWidth");
    this->_rect.height += getInteger("lineWidth");
    rectToQML(ostream);

    
    if (getBool("closePolygon") || getBool("fill"))
    ostream << indent << "    foreground: " << getColor("fillColor") << std::endl;
    else
    ostream << indent << "    foreground: " << getColor("lineColor") << std::endl;
    if (getBool("lineAlarm") || getBool("fillAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

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
        ostream << "Qt.point(" << std::stoi(xp[i]) - this->_rect.x + getInteger("lineWidth")/2<< ","
            << std::stoi(yp[i]) - this->_rect.y + getInteger("lineWidth")/2<< ")";
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
    if (displaySource == "stringPv") {
        auto filePv = getPv("filePv");
        ostream << indent << "    property var filePv: CSData {" << std::endl;
        ostream << indent << "        source: '" << filePv << "'" << std::endl;
        ostream << indent << "    }" << std::endl;
        ostream << indent << "    source: filePv.value" << std::endl;
    } else if  (displaySource == "menu") {
        auto filenames = getList("displayFileName");
        auto macros = getList("symbols");
        auto filePv = getPv("filePv");

        ostream << indent << "    property var fileNames: ";
        for(int i=0; i<filenames.size(); i++)  {
            // append ".edl" suffix if necessary
            std::string filename = filenames[i];
            if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".edl")
                filename += ".edl";
    
            if (i == 0)
                ostream << "[";
            else
                ostream << ",";
            ostream << "'" << filename << "'";
            if (i == filenames.size() -1)
                ostream << "]";
        }
        ostream << std::endl;

        ostream << indent << "    property var macros: ";
        listToQML(ostream, macros);
        ostream << std::endl;

        ostream << indent << "    property var filePv: CSData {" << std::endl;
        ostream << indent << "        source: '" << filePv << "'" << std::endl;
        ostream << indent << "    }" << std::endl;

        ostream << indent << "    source: fileNames[filePv.value] || ''" << std::endl;
        ostream << indent << "    macro: macros[filePv.value] || ''" << std::endl;
    } else if (displaySource == "file") {
        std::string filename = getText("file");
        if (!filename.empty()) {
            if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".edl")
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
    if (getBool("lineAlarm") || getBool("fillAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

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

    ostream << indent << "    label: '" << (getBool("icon")?"":"-") << getText("buttonLabel") << "'" << std::endl;

    auto entries = getList("displayFileName");
    auto labels = getList("menuLabel");
    auto symbols = getList("symbols");
    auto closeActions = getList("closeAction");

    if (entries.size() > 0) {
        ostream << indent << "    model: ListModel {" << std::endl;
        for(int i=0; i<entries.size(); i++) {
            ostream << indent << "        ListElement {" << std::endl;
            std::string filename = entries[i];
            if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".edl")
                filename += ".edl";
            ostream << indent << "              file: '" <<  filename << "'" << std::endl;
            ostream << indent << "              label: '" << (i < labels.size() ? labels[i] : "") << "'" << std::endl;
            std::string macro = screen()->macroString();
            if (i < symbols.size() && !symbols[i].empty())
                macro += (macro.empty() ? "" : ",") + symbols[i]; 
            if (!macro.empty())
            ostream << indent << "              macro: '" << macro << "'" << std::endl;
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

    std::string align = getText("fontAlign");
    if (align == "left")
        ostream << indent << "    align: Text.AlignLeft" << std::endl;
    else if (align == "center")
        ostream << indent << "    align: Text.AlignHCenter" << std::endl;
    else if (align == "right")
        ostream << indent << "    align: Text.AlignRight" << std::endl;

    attr = getFont("font");
    if (!attr.empty())
        ostream << indent << "    " << attr << std::endl;
    
    ostream << indent << "    fontSizeMode: Text.FixedSize" << std::endl;

    attr = getColor("fgColor");
    if (!attr.empty())
        ostream << indent << "    foreground: " <<  attr << std::endl;

    if (!getBool("useDisplayBg")) {
        attr = getColor("bgColor");
        if (!attr.empty())
            ostream << indent << "    background: " <<  attr << std::endl;
    }

    if (getBool("fgAlarm") || getBool("bgAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

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

void Object::cartesianPlotToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSCartesianPlot {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;
    ostream << indent << "    title: '" << getText("graphTitle") << "'" << std::endl;
    ostream << indent << "    xLabel: '" << getText("xLabel") << "'" << std::endl;
    ostream << indent << "    yLabel: '" << getText("yLabel") << "'" << std::endl;

    std::string xAxisSrc = getText("xAxisSrc");
    if (xAxisSrc == "AutoScale")
    ostream << indent << "    xRangeStyle: CartesianPlotRangeStyle.Auto" << std::endl;
    else if (xAxisSrc == "fromUser")
    ostream << indent << "    xRangeStyle: CartesianPlotRangeStyle.User" << std::endl;
    else
    ostream << indent << "    xRangeStyle: CartesianPlotRangeStyle.Channel" << std::endl;
    ostream << indent << "    xRangeLower: " << getInteger("xMin") << std::endl;
    ostream << indent << "    xRangeUpper: " << getInteger("xMax") << std::endl;

    std::string yAxisSrc = getText("yAxisSrc");
    if (yAxisSrc == "AutoScale")
    ostream << indent << "    yRangeStyle: CartesianPlotRangeStyle.Auto" << std::endl;
    else if (yAxisSrc == "fromUser")
    ostream << indent << "    yRangeStyle: CartesianPlotRangeStyle.User" << std::endl;
    else
    ostream << indent << "    yRangeStyle: CartesianPlotRangeStyle.Channel" << std::endl;
    ostream << indent << "    yRangeLower: " << getInteger("yMin") << std::endl;
    ostream << indent << "    yRangeUpper: " << getInteger("yMax") << std::endl;

    std::string y2AxisSrc = getText("y2AxisSrc");
    if (y2AxisSrc == "AutoScale")
    ostream << indent << "    y2RangeStyle: CartesianPlotRangeStyle.Auto" << std::endl;
    else if (y2AxisSrc == "fromUser")
    ostream << indent << "    y2RangeStyle: CartesianPlotRangeStyle.User" << std::endl;
    else
    ostream << indent << "    y2RangeStyle: CartesianPlotRangeStyle.Channel" << std::endl;
    ostream << indent << "    y2RangeLower: " << getInteger("y2Min") << std::endl;
    ostream << indent << "    y2RangeUpper: " << getInteger("y2Max") << std::endl;

    int numTraces = getInteger("numTraces");
    auto xpvs = getPvList("xPv");
    auto ypvs = getPvList("yPv");
    auto colors = getColorList("plotColor");
    auto y2axis = getList("useY2Axis");
    if (numTraces > 0) {
        ostream << indent << "    model: ListModel {" << std::endl;
        for (int i=0; i<numTraces; i++) {
            ostream << indent << "        ListElement {" << std::endl;
            if (i<xpvs.size())
            ostream << indent << "            xchannel: '" << xpvs[i] << "'" << std::endl;
            if (i<ypvs.size())
            ostream << indent << "            ychannel: '" << ypvs[i] << "'" << std::endl;
            if (i<colors.size())
            ostream << indent << "            color: " << colors[i] << std::endl;
            if (i<y2axis.size())
            ostream << indent << "            yaxis: " << y2axis[i] << std::endl;
            ostream << indent << "        }" << std::endl;
        }
        ostream << indent << "    }" << std::endl;
    }
    
    ostream << indent << "    count: " << getInteger("nPts") << std::endl;

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

void Object::stripChartToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSStripChart {" << std::endl;
    rectToQML(ostream);

    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;

    int numPvs = getInteger("numPvs");
    auto pvs = getPvList("yPv");
    auto colors = getColorList("plotColor");
    if (numPvs > 0) {
        ostream << indent << "    model: ListModel {" << std::endl;
        for (int i=0; i<numPvs; i++) {
            ostream << indent << "        ListElement {" << std::endl;
            ostream << indent << "            channel: '" << pvs[i] << "'" << std::endl;
            ostream << indent << "            color: " << colors[i] << std::endl;
            ostream << indent << "        }" << std::endl;
        }
        ostream << indent << "    }" << std::endl;
    }
    ostream << indent << "    period: " << getInteger("updateTime") << std::endl;

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

    std::string format = getText("format");
    std::string displayMode = getText("displayMode");
    if (format == "decimal" || displayMode == "decimal")
    ostream << indent << "    format: TextFormat.Decimal" << std::endl;
    else if (format == "hex" || displayMode == "hex")
    ostream << indent << "    format: TextFormat.Hexadecimal" << std::endl;
    else if (format == "engineer" || displayMode == "engineer")
    ostream << indent << "    format: TextFormat.EngNotation" << std::endl;
    else if (format == "exp" || displayMode == "exp")
    ostream << indent << "    format: TextFormat.Exponential" << std::endl;
    else
    ostream << indent << "    format: TextFormat.String" << std::endl;

    if (!getText("precision").empty()) {
        ostream << indent << "    limits.precSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.precDefault: " << getInteger("precision") << std::endl;
    }

    std::string font = getFont("font");
    if (!font.empty())
        ostream << indent << "    " << font << std::endl;

    ostream << indent << "    fontSizeMode: Text.FixedSize" << std::endl;

    std::string align = getText("fontAlign");
    if (align == "left")
        ostream << indent << "    align: Text.AlignLeft" << std::endl;
    else if (align == "center")
        ostream << indent << "    align: Text.AlignHCenter" << std::endl;
    else if (align == "right")
        ostream << indent << "    align: Text.AlignRight" << std::endl;

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

    std::string orien = getText("orientation");
    if (orien == "horizontal")
        ostream << indent << "    stacking: Stacking.Row" << std::endl;

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
    ostream << indent << "    pressMessage: '" << pressValue << "'" << std::endl;
    std::string releaseValue = getText("releaseValue");
    if (!releaseValue.empty())
    ostream << indent << "    releaseMessage: '" << releaseValue << "'" << std::endl;

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

    if (!getBool("limitsFromDb")) {
        ostream << indent << "    limits.loprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.hoprSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.precSrc: LimitsSource.Default" << std::endl;

        ostream << indent << "    limits.loprDefault: " << getInteger("scaleMin") << std::endl;
        ostream << indent << "    limits.hoprDefault: " << getInteger("scaleMax", 1) << std::endl;
        ostream << indent << "    limits.precDefault: " << getInteger("precision") << std::endl;
    }

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;

    ostream << indent << "}" << std::endl;
}

void Object::textEntryToQML(std::ostream& ostream)
{
    int indent_level = level();
    std::string indent(indent_level * 4, ' ');

    ostream << indent << "CSTextEntry {" << std::endl;

    rectToQML(ostream);

    ostream << indent << "    source: '" << getPv("controlPv") << "'" << std::endl;
    ostream << indent << "    foreground: " << getColor("fgColor") << std::endl;
    ostream << indent << "    background: " << getColor("bgColor") << std::endl;
    if (getBool("fgAlarm"))
    ostream << indent << "    colorMode: ColorMode.Alarm" << std::endl;

    std::string format = getText("format");
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

    if (!getText("precision").empty() || !getBool("limitsFromDb")) {
        ostream << indent << "    limits.precSrc: LimitsSource.Default" << std::endl;
        ostream << indent << "    limits.precDefault: " << getInteger("precision") << std::endl;
    }

    std::string align = getText("fontAlign");
    if (align == "left")
        ostream << indent << "    align: Text.AlignLeft" << std::endl;
    else if (align == "center")
        ostream << indent << "    align: Text.AlignHCenter" << std::endl;
    else if (align == "right")
        ostream << indent << "    align: Text.AlignRight" << std::endl;

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

        ostream << indent << "    limits.loprDefault: " << getInteger("scaleMin") << std::endl;
        ostream << indent << "    limits.hoprDefault: " << getInteger("scaleMax", 1) << std::endl;
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

void Object::listToQML(std::ostream& ostream, std::vector<std::string> list)
{
    ostream << "[";
    for(int i=0; i<list.size(); i++)  {
        ostream << "\"" << list[i] << "\"";
        if (i != list.size() - 1)
            ostream << ",";
    }
    ostream << "]";
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
        case EL_CartesianPlot:
        cartesianPlotToQML(ostream);
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
        case EL_StripChart:
        stripChartToQML(ostream);
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
        case EL_Slider:
        sliderToQML(ostream);
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
    parseColors();
}

void Screen :: parseColors()
{
    std::string edmFilesPath = ".";
    if (std::getenv("EDMFILES"))
        edmFilesPath = std::getenv("EDMFILES");
    std::string filename = edmFilesPath + "/colors.list";
    std::ifstream fstream(filename.c_str());

    if (!fstream.is_open() && !edmFilesPath.empty()) {
        std::cerr << "Failed to open colors list \"" << filename << "\"" << std::endl;
        return;
    }

    std::vector<std::string> tokens;

    /* version */
    tokens = getLine(fstream);
    if (tokens.size() != 3) {
        std::cerr << "Wrong color list format" << std::endl;
    }
    int version = std::stoi(tokens[0]) << 16 | std::stoi(tokens[1]) << 8 | std::stoi(tokens[2]);
    if (version < 0x040000) {
        std::cerr << filename << ": color list version " << tokens[0] << "." << tokens[1] << "." << tokens[2] << " is not supported." << std::endl;
        return;
    }
    /* static colors */
    tokens = getLine(fstream);
    while ( tokens.size() > 0) {
        if (tokens[0] == "static") {
            int index = std::stoi(tokens[1]);
            std::string name = tokens[2];

            std::stringstream sstream;
            sstream << "#" << std::setfill('0') << std::hex
                << std::setw(4) << std::stoi(tokens[3])
                << std::setw(4) << std::stoi(tokens[4])
                << std::setw(4) << std::stoi(tokens[5]);
            std::string color = sstream.str();

            colormap[index] = color;
        }
        tokens = getLine(fstream);
    }
}

void Screen :: parse(std::istream &fstream) 
{
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
            if (!found) {
                std::cerr << tokens[1] << " Not Implemented" << std::endl;
                parseAndSkip(fstream);
            }
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
    }
    /* output to QML with item as root */
    ostream << "import QtQuick 2.0\n";
    ostream << "import CSDataQuick.Data 1.0\n";
    ostream << "import CSDataQuick.Components 1.0\n";
    ostream << "import CSDataQuick.Components.Private 1.0\n";
    ostream << "Item {\n";
    ostream << "    anchors.fill: parent\n";
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        (*it)->toQML(ostream);
    }
    ostream << "}" << std::endl;
}

} // namespace EDL
