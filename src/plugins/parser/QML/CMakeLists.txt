set(SOURCES
    QMLParser.cpp
)

set(HEADERS
    QMLParser.h
)

add_library(QMLParser MODULE
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(QMLParser PRIVATE
    Qt${QT_VERSION_MAJOR}::Core csparser
)

set_target_properties(QMLParser PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/csparser
)

if (APPLE)
    set_target_properties(QMLParser PROPERTIES
    SUFFIX ".dylib")
endif ()

install(TARGETS QMLParser DESTINATION ${KDE_INSTALL_PLUGINDIR}/csparser)
