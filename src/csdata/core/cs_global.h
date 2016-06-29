#ifndef CS_GLOBAL_H
#define CS_GLOBAL_H

#ifdef Q_OS_WIN32
    #if defined(QCS_EXPORT)
        #define QCS_DLLSPEC Q_DECL_EXPORT
    #else
        #define QCS_DLLSPEC Q_DECL_IMPORT
    #endif
#else
    #define QCS_DLLSPEC
#endif

#endif // CS_GLOBAL_H
