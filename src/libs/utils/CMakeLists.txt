set(SOURCES
    enums.cpp
    utils.cpp
    conversion.c
    geometryspecification.cpp 
)

set(HEADERS
    enums.h
    utils.h
    utils_global.h
)

add_library(csutils SHARED
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(csutils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(csutils PRIVATE
    UTILS_LIBRARY
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_link_libraries(csutils
    PUBLIC
        Qt5::Core
        Qt5::Qml
        EPICS::Com
    PRIVATE
        Qt5::Gui
        Qt5::Quick
        parser
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
