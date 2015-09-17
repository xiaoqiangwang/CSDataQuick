#include "parser.h"
#include "element.h"

#include <fstream>
#include <sstream>

std::string parseADLDisplay(std::string filename, std::map<std::string, std::string> macros)
{
    Display displayInfo(0);
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);

    std::ifstream ifstream(filename.c_str());
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    displayInfo.toQML(osstream);
    osstream.flush(); 

    return osstream.str();
}

std::string parseADLComposite(std::string filename, std::map<std::string, std::string> macros)
{
    Display displayInfo(0);
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);

    std::ifstream ifstream(filename.c_str());
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    displayInfo.toPartialQML(osstream);
    osstream.flush();

    return osstream.str();
}
