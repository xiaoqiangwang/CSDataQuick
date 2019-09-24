set(SOURCES
    csdata.cpp
    csdataengine.cpp
    csdataenginemanager.cpp
    objectmodel.cpp
)

set(HEADERS
    csdata_global.h
    csdata.h
    csdataengine.h
    csdataenginemanager.h
    objectmodel.h
)

add_library(csdata SHARED
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(csdata PRIVATE CSDATA_LIBRARY)

target_link_libraries(csdata PUBLIC Qt5::Core)

set_target_properties(csdata PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS csdata
    RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
)
