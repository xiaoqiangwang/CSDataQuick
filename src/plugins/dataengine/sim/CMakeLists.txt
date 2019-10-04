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

target_link_libraries(csdataenginesim PRIVATE
    Qt5::Core
    csdata
)

set_target_properties(csdataenginesim PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csdataengine)

if (APPLE)
set_target_properties(csdataenginesim PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS csdataenginesim DESTINATION plugins/csdataengine)
