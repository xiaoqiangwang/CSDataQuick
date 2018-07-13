#ifndef UIPARSER_H
#define UIPARSER_H

#include <string>
#include <istream>
#include <map>
#include "UIParser_global.h"

UIPARSER_EXPORT std::string parseUIDisplay(std::string filename, std::map<std::string, std::string> macros);
UIPARSER_EXPORT std::string parseUIComposite(std::string filename, std::map<std::string, std::string> macros);

#endif
