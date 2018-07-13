#include "UIParser.h"
#include "parser.h"

#include <fstream>
#include <sstream>

std::string parseUIDisplay(std::string filename, std::map<std::string, std::string> macros)
{
    UI displayInfo;
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);

    QXmlStreamReader reader(filename.c_str());
    displayInfo.parse(reader);

    std::ostringstream osstream;
    displayInfo.toQML(osstream);
    osstream.flush(); 

    return osstream.str();
}

std::string parseUIComposite(std::string filename, std::map<std::string, std::string> macros)
{
    UI displayInfo;
    displayInfo.setFileName(filename);
    displayInfo.setMacros(macros);

    QXmlStreamReader reader(filename.c_str());
    displayInfo.parse(reader);

    std::ostringstream osstream;
    displayInfo.toPartialQML(osstream);
    osstream.flush();

    return osstream.str();
}
