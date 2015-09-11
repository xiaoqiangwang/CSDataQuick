#ifndef PARSER_H
#define PARSER_H

#include <string>
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

PARSER_DLLSPEC std::string parseADL(std::string filename, std::map<std::string, std::string> macros);

#endif
