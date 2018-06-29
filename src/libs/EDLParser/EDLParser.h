#ifndef EDLPARSER_H
#define EDLPARSER_H

#include <string>
#include <istream>
#include <map>
#include "EDLParser_global.h"

EDLPARSER_EXPORT std::string parseEDLDisplay(std::string filename, std::map<std::string, std::string> macros);
EDLPARSER_EXPORT std::string parseEDLComposite(std::string filename, std::map<std::string, std::string> macros);

#endif
