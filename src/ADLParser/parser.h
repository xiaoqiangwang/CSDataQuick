#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <istream>
#include <map>

#if defined(_WIN32)
    #if defined EXPORT_SHAREDLIB
        #define PARSER_DLLSPEC __declspec(dllexport)
    #else
        #define PARSER_DLLSPEC __declspec(dllimport)
    #endif
#else
    #define PARSER_DLLSPEC
#endif

PARSER_DLLSPEC std::string parseADLDisplay(std::string filename, std::map<std::string, std::string> macros);
PARSER_DLLSPEC std::string parseADLComposite(std::string filename, std::map<std::string, std::string> macros);

#endif
