#include "parser.h"
#include "element.h"
#include "fileinfo.h"

#include <sstream>

std::string parseADL(std::istream &ifstream, std::map<std::string, std::string> macros)
{
    Display displayInfo(0);
    //displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    displayInfo.toQML(osstream);
    osstream.flush(); 

    return osstream.str();
}

std::string parseCompositeADL(std::istream &ifstream, std::map<std::string, std::string> macros)
{
    Display displayInfo(0);
    //displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    displayInfo.toPartialQML(osstream);
    osstream.flush();

    return osstream.str();
}
