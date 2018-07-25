#pragma once

#include <qglobal.h>

#if defined(PARSER_LIBRARY)
#  define PARSER_EXPORT Q_DECL_EXPORT
#else
#  define PARSER_EXPORT Q_DECL_IMPORT
#endif
