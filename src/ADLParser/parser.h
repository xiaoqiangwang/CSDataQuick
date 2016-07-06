#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <istream>
#include <map>
#include "cs_global.h"

QCS_DLLSPEC std::string parseADLDisplay(std::string filename, std::map<std::string, std::string> macros);
QCS_DLLSPEC std::string parseADLComposite(std::string filename, std::map<std::string, std::string> macros);

#endif
