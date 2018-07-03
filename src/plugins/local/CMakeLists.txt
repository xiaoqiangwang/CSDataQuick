set(SOURCES
    csdataenginelocal.cpp
)

set(HEADERS
    csdataenginelocal.h
)

add_library(csdataenginelocal MODULE
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

target_link_libraries(csdataenginelocal Qt5::Core)
target_link_libraries(csdataenginelocal csdata)
set_target_properties(csdataenginelocal PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csdataengine)

if (APPLE)
set_target_properties(csdataenginelocal PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS csdataenginelocal DESTINATION plugins/csdataengine)
