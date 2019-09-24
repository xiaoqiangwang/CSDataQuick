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

target_include_directories(csdataengineca PRIVATE
    ${EPICS_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

target_compile_definitions(csdataengineca PRIVATE
    ${EPICS_DEFINITIONS}
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_link_libraries(csdataengineca PRIVATE
    Qt5::Core
    ${EPICS_LIBRARIES}
    csdata
)

set_target_properties(csdataengineca PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csdataengine)

if (APPLE)
set_target_properties(csdataengineca PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS csdataengineca DESTINATION plugins/csdataengine)
