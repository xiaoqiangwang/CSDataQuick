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

target_link_libraries(csdataenginelocal PRIVATE
    Qt5::Core
    csdata
)

set_target_properties(csdataenginelocal PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csdataengine)

if (APPLE)
set_target_properties(csdataenginelocal PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS csdataenginelocal DESTINATION plugins/csdataengine)
