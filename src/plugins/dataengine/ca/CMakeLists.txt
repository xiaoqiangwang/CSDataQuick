set(SOURCES
    csdataengineca.cpp
)

set(HEADERS
    csdataengineca.h
)

add_library(csdataengineca MODULE
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${EPICS_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

target_compile_definitions(csdataengineca PRIVATE
    ${EPICS_DEFINITIONS}
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)


target_link_libraries(csdataengineca Qt5::Core)
target_link_libraries(csdataengineca ${EPICS_LIBRARIES})
target_link_libraries(csdataengineca csdata)
set_target_properties(csdataengineca PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csdataengine)

if (APPLE)
set_target_properties(csdataengineca PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS csdataengineca DESTINATION plugins/csdataengine)
