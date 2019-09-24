set(SOURCES
    enums.cpp
    utils.cpp
    conversion.c
    geometryspecification.cpp 
)

set(HEADERS
    enums.h
    utils.h
    utils_global.h
)

add_library(csutils SHARED
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(csutils PRIVATE
    UTILS_LIBRARY
    ${EPICS_DEFINITIONS}
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

target_include_directories(csutils PRIVATE
    ${EPICS_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/libs/parser
    ${CMAKE_SOURCE_DIR}/src/libs/csdata
)

target_link_libraries(csutils
    PUBLIC
        Qt5::Core
        Qt5::Qml
        ${EPICS_LIBRARIES}
    PRIVATE
        Qt5::Gui
        Qt5::Quick
        parser
        csdata
)

set_target_properties(csutils PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS csutils
    RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
)
