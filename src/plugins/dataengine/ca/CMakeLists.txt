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

target_compile_definitions(csdataengineca PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_link_libraries(csdataengineca PRIVATE
    Qt5::Core
    csdata
    EPICS::ca
)

set_target_properties(csdataengineca PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csdataengine)

if (APPLE)
set_target_properties(csdataengineca PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS csdataengineca DESTINATION ${KDE_INSTALL_PLUGINDIR}/csdataengine)
