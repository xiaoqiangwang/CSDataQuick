# Reads:
#   EPICS_USE_STATIC_LIBRARIES: Prefer linking to static EPICS libraries.
#
# Sets:
#   EPICS_FOUND: TRUE if epics base is found.
#   EPICS_VERSION: epics base version string.
#   EPICS_INCLUDE_DIRS: Include directories for epics base.
#   EPICS_CA_LIBRARIES: epics ca client libraries required by the linker.
#   EPICS_PVA_LIBRARIES: epics pva client libraries required by the linker.
#   EPICS_COMPILE_DEFINITIONS: Compiler definitions.
#
# Hints:
#   EPICS_BASE: epics base directopry
#   EPICS_HOST_ARCH: epics host architecture


# set a var only if it is neither defined or in environment
macro(set_var name value)
    if (NOT ${name})
        if (DEFINED ENV{${name}})
            set (${name} $ENV{${name}})
        else ()
            set (${name} ${value})
        endif ()
    endif ()
endmacro()

# Derive EPICS_HOST_ARCH from environment variable "EPICS_HOST_ARCH"
# Otherwuse guess EPICS_OS_CLASS, EPICS_COMPL_CLASS, EPICS_HOST_ARCH
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_var(EPICS_OS_CLASS "WIN32")
    set_var(EPICS_CMPL_CLASS "msvc")
    if (EPICS_USE_STATIC_LIBRARIES)
        set(_suffix "-static")
    endif ()
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set_var(EPICS_HOST_ARCH "windows-x64${_suffix}")
    else ()
        set_var(EPICS_HOST_ARCH "win32-x86${_suffix}")
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_var(EPICS_OS_CLASS "Darwin")
    set_var(EPICS_CMPL_CLASS "clang")
    set_var(EPICS_HOST_ARCH "darwin-x86")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_var(EPICS_OS_CLASS "Linux")
    set_var(EPICS_CMPL_CLASS "gcc")
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set_var(EPICS_HOST_ARCH "linux-x86_64")
    else ()
        set_var(EPICS_HOST_ARCH "linux-x86")
    endif ()
endif ()


# Derive EPICS_BASE from evironment variable "EPICS_BASE" and "EPICS"
if (NOT EPICS_BASE)
    file (GLOB __epics_base_dirs
        $ENV{EPICS}/base*
        /usr/local/epics/base*
        /opt/local/epics/base*
        C:/epics/base*
    )
    find_path(EPICS_BASE include/epicsVersion.h
        PATHS ${__epics_base_dirs}
        NO_DEFAULT_PATH
    )
endif ()

SET (EPICS_INCLUDE_DIRS
    ${EPICS_BASE}/include
    ${EPICS_BASE}/include/os/${EPICS_OS_CLASS}
    ${EPICS_BASE}/include/compiler/${EPICS_CMPL_CLASS}
)

SET (EPICS_LIBRARY_DIRS
    ${EPICS_BASE}/lib/${EPICS_HOST_ARCH}
)

SET (EPICS_BINARY_DIRS
    ${EPICS_BASE}/bin/${EPICS_HOST_ARCH}
)

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
        IF (${EPICS_VERSION} VERSION_LESS 3.15)
            LIST (REMOVE_ITEM EPICS_INCLUDE_DIRS ${EPICS_BASE}/include/compiler/${EPICS_CMPL_CLASS})
        ENDIF ()
    ENDIF()
ENDIF()

# Find EPICS libraries
foreach (LIB Com ca pvData pvAccess)
    if (EPICS_USE_STATIC_LIBRARIES)
        set (THIS_LIB lib${LIB}.a)
    else ()
        set (THIS_LIB lib${LIB}.so lib${LIB}.dylib)
    endif ()
    find_library (EPICS_${LIB}_LIBRARY
        NAMES ${THIS_LIB} ${LIB}
        HINTS ${EPICS_LIBRARY_DIRS})
    if (NOT EPICS_USE_STATIC_LIBRARIES)
        find_file(EPICS_${LIB}_SHARED_LIBRARY
            NAMES lib${LIB}.so lib${LIB}.dylib ${LIB}.dll
            HINTS ${EPICS_LIBRARY_DIRS} ${EPICS_BINARY_DIRS})
    endif ()
endforeach()

# Check compiler flags and additional libraries 
if (EPICS_Com_LIBRARY)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        execute_process(COMMAND nm -u ${EPICS_Com_LIBRARY}
                        COMMAND grep "rl_"
                        OUTPUT_VARIABLE WITH_READLINE)
        if (WITH_READLINE)
            list(APPEND EPICS_Com_DEPENDS "readline")
        endif ()
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if (EPICS_HOST_ARCH MATCHES "^win.*-static$")
            set(EPICS_COMPILE_DEFINITIONS "EPICS_DLL_NO")
            list(APPEND EPICS_Com_DEPENDS "ws2_32")
            foreach(flag_var
                    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
                    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
               if(${flag_var} MATCHES "/MD")
                   string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
               endif(${flag_var} MATCHES "/MD")
            endforeach(flag_var)
        else () 
            set(EPICS_COMPILE_DEFINITIONS "EPICS_CALL_DLL")
        endif ()
    endif ()
endif ()

if (EPICS_ca_LIBRARY)
    set (EPICS_CA_LIBRARIES ${EPICS_Com_LIBRARY} ${EPICS_ca_LIBRARY})
    set (EPICS_CA_FOUND TRUE)
endif ()

if (EPICS_pvAccess_LIBRARY)
    set (EPICS_PVA_LIBRARIES ${EPICS_Com_LIBRARY} ${EPICS_pvData_LIBRARY} ${EPICS_pvAccess_LIBRARY})
    set (EPICS_PVA_FOUND TRUE)
endif ()

set (EPICS_REQUIRED_LIBRARIES EPICS_CA_LIBRARIES)
if (${EPICS_VERSION} AND ${EPICS_VERSION} VERSION_GREATER 7)
    list (APPEND EPICS_REQUIRED_LIBRARIES EPICS_PVA_LIBRARIES)
endif ()

# ----------------------------------------------------------------------------
# handle the QUIETLY and REQUIRED arguments and set *_FOUND to TRUE
# if all listed variables are found or TRUE
include (FindPackageHandleStandardArgs)

find_package_handle_standard_args (
    EPICS
    REQUIRED_VARS  EPICS_BASE EPICS_INCLUDE_DIRS EPICS_Com_LIBRARY ${EPICS_REQUIRED_LIBRARIES}
    VERSION_VAR EPICS_VERSION
)

if (EPICS_FOUND)
    if (NOT TARGET EPICS::Com)
        add_library(EPICS::Com UNKNOWN IMPORTED)
        set_property(TARGET EPICS::Com PROPERTY IMPORTED_LOCATION  ${EPICS_Com_LIBRARY})
        set_property(TARGET EPICS::Com APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${EPICS_INCLUDE_DIRS})
        set_property(TARGET EPICS::Com APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS ${EPICS_COMPILE_DEFINITIONS})
        set_property(TARGET EPICS::Com APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${EPICS_Com_DEPENDS})
    endif ()
    if (NOT TARGET EPICS::ca)
        add_library(EPICS::ca UNKNOWN IMPORTED)
        set_property(TARGET EPICS::ca PROPERTY IMPORTED_LOCATION  ${EPICS_ca_LIBRARY})
        set_property(TARGET EPICS::ca APPEND PROPERTY INTERFACE_LINK_LIBRARIES EPICS::Com)
    endif ()
    if (NOT TARGET EPICS::pva AND ${EPICS_pvAccess_LIBRARY})
        add_library(EPICS::pva UNKNOWN IMPORTED)
        set_property(TARGET EPICS::pva PROPERTY IMPORTED_LOCATION ${EPICS_pvAccess_LIBRARY})
        set_property(TARGET EPICS::pva APPEND PROPERTY INTERFACE_LINK_LIBRARIES EPICS::Com)
        set_property(TARGET EPICS::pva APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${EPICS_pvData_LIBRARY})
    endif ()
endif ()
