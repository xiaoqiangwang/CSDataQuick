#include "parser.h"
#include "element.h"

#include <fstream>
#include <sstream>

std::string parseADL(std::string filename, std::map<std::string, std::string> macros)
{
    std::fstream ifstream;
    ifstream.open(filename.c_str(), std::fstream::in);

    Display displayInfo(0);
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);
    displayInfo.parse(ifstream);

    std::ostringstream osstream;
    displayInfo.toQML(osstream);
    osstream.flush(); 

    return osstream.str();
}
