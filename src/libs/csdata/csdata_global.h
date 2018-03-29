#pragma once

#include <qglobal.h>

#if defined(CSDATA_LIBRARY)
#  define CSDATA_EXPORT Q_DECL_EXPORT
#else
#  define CSDATA_EXPORT Q_DECL_IMPORT
#endif
