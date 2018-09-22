set(SOURCES
    csdataenginesim.cpp
)

set(HEADERS
    csdataenginesim.h
)

add_library(csdataenginesim MODULE
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

target_link_libraries(csdataenginesim Qt5::Core)
target_link_libraries(csdataenginesim csdata)
set_target_properties(csdataenginesim PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csdataengine)

if (APPLE)
set_target_properties(csdataenginesim PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS csdataenginesim DESTINATION plugins/csdataengine)
