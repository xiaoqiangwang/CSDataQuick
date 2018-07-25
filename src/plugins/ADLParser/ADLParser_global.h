#pragma once

#include <qglobal.h>

#if defined(ADLPARSER_LIBRARY)
#  define ADLPARSER_EXPORT Q_DECL_EXPORT
#else
#  define ADLPARSER_EXPORT Q_DECL_IMPORT
#endif
