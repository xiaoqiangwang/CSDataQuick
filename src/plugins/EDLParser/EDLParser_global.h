#pragma once

#include <qglobal.h>

#if defined(EDLPARSER_LIBRARY)
#  define EDLPARSER_EXPORT Q_DECL_EXPORT
#else
#  define EDLPARSER_EXPORT Q_DECL_IMPORT
#endif
