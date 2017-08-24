SET(EPICS_BASE $ENV{EPICS_BASE})
IF (NOT EPICS_BASE)
    SET (EPICS_ROOT $ENV{EPICS})
    IF (EPICS_ROOT)
        SET(EPICS_BASE "${EPICS_ROOT}/base")
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
ELSEIF (UNIX)
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

FIND_LIBRARY (EPICS_CA_LIBRARY NAMES libca.a ca HINTS ${EPICS_LIBRARY_DIRS})
FIND_LIBRARY (EPICS_COM_LIBRARY NAMES libCom.a Com HINTS ${EPICS_LIBRARY_DIRS})
SET(EPICS_LIBRARIES ${EPICS_CA_LIBRARY} ${EPICS_COM_LIBRARY})

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
