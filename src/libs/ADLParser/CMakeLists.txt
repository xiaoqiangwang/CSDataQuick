set(SOURCES
    element.cpp
    ADLParser.cpp
)

set(HEADERS
    common.h
    element.h
    ADLParser.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

add_library(ADLParser SHARED
    ${SOURCES}
    ${HEADERS}
)

if (WIN32)
    set_target_properties(ADLParser PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    install(TARGETS ADLParser DESTINATION bin)
else ()
    set_target_properties(ADLParser PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    install(TARGETS ADLParser DESTINATION lib)
endif ()
