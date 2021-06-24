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
#   EPICS_COMPILE_OPTIONS: Compiler options.
#   EPICS_LINK_OPTIONS: Linker options.
#
# Hints:
#   EPICS_BASE: epics base directopry
#   EPICS_HOST_ARCH: epics host architecture

# set a var only if it is neither defined or in environment
macro(_set_var name value)
    if (NOT ${name})
        if (DEFINED ENV{${name}})
            set (${name} $ENV{${name}})
        else ()
            set (${name} ${value})
        endif ()
    endif ()
endmacro()

# add imported library target
macro (_add_imported_library name type)
    string(SUBSTRING ${name} 7 -1 _name)
    add_library(${name} ${type} IMPORTED)
    set_property(TARGET ${name} PROPERTY IMPORTED_LOCATION ${EPICS_${_name}_LIBRARY})
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND ${type} STREQUAL "SHARED")
        set_property(TARGET ${name} PROPERTY IMPORTED_IMPLIB ${EPICS_${_name}_LIBRARY})
        set_property(TARGET ${name} PROPERTY IMPORTED_LOCATION ${EPICS_${_name}_SHARED_LIBRARY})
    endif ()
    set_property(TARGET ${name} PROPERTY INTERFACE_LINK_LIBRARIES ${EPICS_${_name}_DEPENDS})
endmacro ()

# Derive EPICS_HOST_ARCH from environment variable "EPICS_HOST_ARCH"
# Otherwuse guess EPICS_OS_CLASS, EPICS_COMPL_CLASS, EPICS_HOST_ARCH
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    _set_var(EPICS_OS_CLASS "WIN32")
    _set_var(EPICS_CMPL_CLASS "msvc")
    if (EPICS_USE_STATIC_LIBRARIES)
        set(_suffix "-static")
    endif ()
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        _set_var(EPICS_HOST_ARCH "windows-x64${_suffix}")
    else ()
        _set_var(EPICS_HOST_ARCH "win32-x86${_suffix}")
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    _set_var(EPICS_OS_CLASS "Darwin")
    _set_var(EPICS_CMPL_CLASS "clang")
    _set_var(EPICS_HOST_ARCH "darwin-x86")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    _set_var(EPICS_OS_CLASS "Linux")
    _set_var(EPICS_CMPL_CLASS "gcc")
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        _set_var(EPICS_HOST_ARCH "linux-x86_64")
    else ()
        _set_var(EPICS_HOST_ARCH "linux-x86")
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
        PATHS $ENV{EPICS_BASE} ${__epics_base_dirs}
        NO_DEFAULT_PATH
    )
endif ()

set (EPICS_INCLUDE_DIRS
    ${EPICS_BASE}/include
    ${EPICS_BASE}/include/os/${EPICS_OS_CLASS}
    ${EPICS_BASE}/include/compiler/${EPICS_CMPL_CLASS}
)

set (EPICS_LIBRARY_DIRS
    ${EPICS_BASE}/lib/${EPICS_HOST_ARCH}
)

set (EPICS_BINARY_DIRS
    ${EPICS_BASE}/bin/${EPICS_HOST_ARCH}
)

# Find EPICS version
if (EXISTS ${EPICS_BASE}/include/epicsVersion.h)
    file (READ ${EPICS_BASE}/include/epicsVersion.h EPICS_VERSION_SRC)
    string (REGEX MATCH
        "#define EPICS_VERSION +([0-9]+)\n#define EPICS_REVISION +([0-9]+)\n#define EPICS_MODIFICATION +([0-9]+)\n#define EPICS_PATCH_LEVEL +([0-9]+)"
        EPICS_VERSION_STR
        ${EPICS_VERSION_SRC}
    )
    if (EPICS_VERSION_STR)
        set( EPICS_VERSION ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}.${CMAKE_MATCH_4})
        if (${EPICS_VERSION} VERSION_LESS 3.15)
            list (REMOVE_ITEM EPICS_INCLUDE_DIRS ${EPICS_BASE}/include/compiler/${EPICS_CMPL_CLASS})
        endif ()
    endif ()
endif ()

# Find EPICS libraries
foreach (LIB Com ca pvData pvAccess)
    if (EPICS_USE_STATIC_LIBRARIES)
        set (THIS_LIB lib${LIB}.a)
    else ()
        set (THIS_LIB lib${LIB}.so lib${LIB}.dylib)
    endif ()
    find_library (EPICS_${LIB}_LIBRARY
        NAMES ${THIS_LIB} ${LIB}
        HINTS ${EPICS_LIBRARY_DIRS}
        NO_DEFAULT_PATH)
    find_file(EPICS_${LIB}_SHARED_LIBRARY
        NAMES lib${LIB}.so lib${LIB}.dylib ${LIB}.dll
        HINTS ${EPICS_LIBRARY_DIRS} ${EPICS_BINARY_DIRS}
        NO_DEFAULT_PATH)
endforeach()

# Be paranoid to check consistency between EPICS_USE_STATIC_LIBRARIES and
# the actual build type of EPICS base.
if (EPICS_USE_STATIC_LIBRARIES)
    set (_LIBTYPE "STATIC")
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND EPICS_Com_SHARED_LIBRARY)
        set (_LIBTYPE "SHARED")
    endif ()
    if (EPICS_Com_LIBRARY STREQUAL EPICS_Com_SHARED_LIBRARY)
        set (_LIBTYPE "SHARED")
    endif ()
else ()
    set (_LIBTYPE "SHARED")
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND NOT EPICS_Com_SHARED_LIBRARY)
        set (_LIBTYPE "STATIC")
    endif ()
endif()

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
        if (_LIBTYPE STREQUAL "STATIC")
            set(EPICS_COMPILE_DEFINITIONS "EPICS_DLL_NO")
            set(EPICS_COMPILE_OPTIONS "/U_DLL")
            set(EPICS_LINK_OPTIONS "/NODEFAULTLIB:libcmt.lib")
            list(APPEND EPICS_Com_DEPENDS "ws2_32")
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
if (${EPICS_VERSION} AND ${EPICS_VERSION} VERSION_GREATER 3)
    list (APPEND EPICS_REQUIRED_LIBRARIES EPICS_PVA_LIBRARIES)
endif ()

# ----------------------------------------------------------------------------
# handle the QUIETLY and REQUIRED arguments and set *_FOUND to TRUE
# if all listed variables are found or TRUE
include (FindPackageHandleStandardArgs)

find_package_handle_standard_args (
    EPICS
    REQUIRED_VARS  EPICS_BASE EPICS_INCLUDE_DIRS ${EPICS_REQUIRED_LIBRARIES}
    VERSION_VAR EPICS_VERSION
)

if (EPICS_FOUND)
    if (NOT TARGET EPICS::Com)
        _add_imported_library(EPICS::Com ${_LIBTYPE})
        set_property(TARGET EPICS::Com APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${EPICS_INCLUDE_DIRS})
        set_property(TARGET EPICS::Com APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS ${EPICS_COMPILE_DEFINITIONS})
        set_property(TARGET EPICS::Com APPEND PROPERTY INTERFACE_COMPILE_OPTIONS ${EPICS_COMPILE_OPTIONS})
        set_property(TARGET EPICS::Com APPEND PROPERTY INTERFACE_LINK_OPTIONS ${EPICS_LINK_OPTIONS})
    endif ()
    if (NOT TARGET EPICS::ca)
        _add_imported_library(EPICS::ca ${_LIBTYPE})
        set_property(TARGET EPICS::ca APPEND PROPERTY INTERFACE_LINK_LIBRARIES EPICS::Com)
    endif ()
    if (NOT TARGET EPICS::pva AND ${EPICS_pvAccess_LIBRARY})
        _add_imported_library(EPICS::pva ${_LIBTYPE})
        set_property(TARGET EPICS::pva APPEND PROPERTY INTERFACE_LINK_LIBRARIES EPICS::Com)
        set_property(TARGET EPICS::pva APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${EPICS_pvData_LIBRARY})
    endif ()
endif ()
