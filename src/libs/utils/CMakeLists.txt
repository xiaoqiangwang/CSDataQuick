set(SOURCES
    libCom/calc/postfix.c
    libCom/calc/calcPerform.c
    libCom/cvtFast/cvtFast.c
    enums.cpp
    utils.cpp
    conversion.c
    geometryspecification.cpp 
)

set(HEADERS
    libCom/calc/postfix.h
    libCom/calc/postfixPvt.h
    libCom/cvtFast/cvtFast.h
    enums.h
    utils.h
    utils_global.h
)

add_library(csutils SHARED
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(csutils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libCom/calc>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libCom/cvtFast>
)

target_compile_definitions(csutils PRIVATE
    UTILS_LIBRARY
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_link_libraries(csutils
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Qml
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Quick
        csparser
        csdata
)

set_target_properties(csutils PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS csutils
    RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
)
