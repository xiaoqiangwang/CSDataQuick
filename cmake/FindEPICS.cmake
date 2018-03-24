# Sets:
#   EPICS_FOUND: TRUE if epics base is found.
#   EPICS_VERSION: epics base version string.
#   EPICS_INCLUDE_DIRS: Include directories for epics base.
#   EPICS_LIBRARIES: epics Com and ca libraries required by the linker.
#   EPICS_SHARED_LIBRARIES: epics Com and ca shared libraries.
#   EPICS_DEFINITIONS: Compiler definitions.

# Derive EPICS_BASE from evironment variable "EPICS_BASE" and "EPICS"
IF (NOT EPICS_BASE)
    IF (DEFINED ENV{EPICS_BASE})
        SET(EPICS_BASE $ENV{EPICS_BASE})
    ELSEIF (DEFINED ENV{EPICS})
        SET(EPICS_BASE $ENV{EPICS}/base)
    ENDIF ()
ENDIF ()

IF (NOT EPICS_HOST_ARCH)
    SET (EPICS_HOST_ARCH $ENV{EPICS_HOST_ARCH})
ENDIF()

IF (WIN32)
    SET(OSCLASS "WIN32")
    SET(CMPLCLASS "msvc")
ELSEIF (APPLE)
    SET(OSCLASS "Darwin")
    SET(CMPLCLASS "clang")
ELSEIF (CMAKE_SYSTEM_NAME STREQUAL Linux)
    SET(OSCLASS "Linux")
    SET(CMPLCLASS "gcc")
ENDIF()

SET (EPICS_INCLUDE_DIRS
    ${EPICS_BASE}/include
    ${EPICS_BASE}/include/os/${OSCLASS} 
    ${EPICS_BASE}/include/compiler/${CMPLCLASS} 
)

SET( EPICS_LIBRARY_DIRS
    ${EPICS_BASE}/lib/${EPICS_HOST_ARCH}
)

SET( EPICS_BINARY_DIRS
    ${EPICS_BASE}/bin/${EPICS_HOST_ARCH}
)

FIND_LIBRARY (EPICS_CA_LIBRARY NAMES libca.a ca HINTS ${EPICS_LIBRARY_DIRS})
FIND_LIBRARY (EPICS_COM_LIBRARY NAMES libCom.a Com HINTS ${EPICS_LIBRARY_DIRS})
SET(EPICS_LIBRARIES ${EPICS_CA_LIBRARY} ${EPICS_COM_LIBRARY})

FIND_FILE (EPICS_CA_SHARED_LIBRARY NAMES ca.dll HINTS ${EPICS_BINARY_DIRS})
FIND_FILE (EPICS_COM_SHARED_LIBRARY NAMES Com.dll HINTS ${EPICS_BINARY_DIRS})
SET(EPICS_SHARED_LIBRARIES ${EPICS_CA_SHARED_LIBRARY} ${EPICS_COM_SHARED_LIBRARY})

IF (CMAKE_SYSTEM_NAME STREQUAL Linux)
    EXECUTE_PROCESS(COMMAND nm -u ${EPICS_COM_LIBRARY}
                    COMMAND grep "rl_"
                    OUTPUT_VARIABLE WITH_READLINE)
    IF (WITH_READLINE)
        LIST(APPEND EPICS_LIBRARIES "readline")
    ENDIF ()
ENDIF ()

SET(EPICS_DEFINITIONS "")
IF (EPICS_HOST_ARCH MATCHES "^win.*-static$")
    SET(EPICS_DEFINITIONS "/D EPICS_DLL_NO")
    LIST(APPEND EPICS_LIBRARIES "ws2_32")
    FOREACH(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
       IF(${flag_var} MATCHES "/MD")
          STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
       ENDIF(${flag_var} MATCHES "/MD")
    ENDFOREACH(flag_var)
ENDIF ()

# Find EPICS version
IF (EXISTS ${EPICS_BASE}/include/epicsVersion.h)
    FILE(READ ${EPICS_BASE}/include/epicsVersion.h EPICS_VERSION_SRC)
    STRING(REGEX MATCH 
        "#define EPICS_VERSION +([0-9]+)\n#define EPICS_REVISION +([0-9]+)\n#define EPICS_MODIFICATION +([0-9]+)\n#define EPICS_PATCH_LEVEL +([0-9]+)"
        EPICS_VERSION_STR
        ${EPICS_VERSION_SRC}
    )
    IF(EPICS_VERSION_STR)
        SET(EPICS_VERSION ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}.${CMAKE_MATCH_4})
    ENDIF()
ENDIF()
# ----------------------------------------------------------------------------
# handle the QUIETLY and REQUIRED arguments and set *_FOUND to TRUE
# if all listed variables are found or TRUE
include (FindPackageHandleStandardArgs)

find_package_handle_standard_args (
    EPICS
    REQUIRED_VARS  EPICS_LIBRARIES EPICS_INCLUDE_DIRS
    VERSION_VAR EPICS_VERSION
)
