#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <istream>
#include <map>

std::string parseADLDisplay(std::string filename, std::map<std::string, std::string> macros);
std::string parseADLComposite(std::string filename, std::map<std::string, std::string> macros);

#endif
