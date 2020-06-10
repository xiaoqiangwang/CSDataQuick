set(SOURCES
    element.cpp
    ADLParser.cpp
)

set(HEADERS
    common.h
    element.h
    ADLParser.h
)

add_library(ADLParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(ADLParser PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_link_libraries(ADLParser PRIVATE
    Qt5::Core csparser
)

set_target_properties(ADLParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csparser
)

if (APPLE)
    set_target_properties(ADLParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS ADLParser DESTINATION ${KDE_INSTALL_PLUGINDIR}/csparser)
