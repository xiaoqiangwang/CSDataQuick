#ifndef ADLPARSER_H
#define ADLPARSER_H

#include <string>
#include <istream>
#include <map>
#include "ADLParser_global.h"

ADLPARSER_EXPORT std::string parseADLDisplay(std::string filename, std::map<std::string, std::string> macros);
ADLPARSER_EXPORT std::string parseADLComposite(std::string filename, std::map<std::string, std::string> macros);

#endif
