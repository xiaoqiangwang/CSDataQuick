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

target_include_directories(csdata PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(csdata PRIVATE
    CSDATA_LIBRARY
    $<$<TARGET_EXISTS:csdataengineca>:WITH_DATAENGINE_CA>
)

target_link_libraries(csdata
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
    PRIVATE
        csdataenginesim
        csdataenginelocal
        $<TARGET_NAME_IF_EXISTS:csdataengineca>
)

set_target_properties(csdata PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS csdata
    RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
)
