#pragma once

#include <qglobal.h>

#if defined(UIPARSER_LIBRARY)
#  define UIPARSER_EXPORT Q_DECL_EXPORT
#else
#  define UIPARSER_EXPORT Q_DECL_IMPORT
#endif
