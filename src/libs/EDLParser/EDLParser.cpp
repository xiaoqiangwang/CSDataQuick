#include "EDLParser.h"
#include "parser.h"

#include <fstream>
#include <sstream>

std::string parseEDLDisplay(std::string filename, std::map<std::string, std::string> macros)
{
    Screen displayInfo;
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);

    std::ifstream ifstream(filename.c_str());
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    displayInfo.toQML(osstream);
    osstream.flush(); 

    return osstream.str();
}

std::string parseEDLComposite(std::string filename, std::map<std::string, std::string> macros)
{
    Screen displayInfo;
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);

    std::ifstream ifstream(filename.c_str());
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    displayInfo.toPartialQML(osstream);
    osstream.flush();

    return osstream.str();
}
